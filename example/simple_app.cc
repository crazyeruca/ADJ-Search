// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
#include "stdafx.h"
#include "simple_app.h"

#include <string>

#include "simple_handler.h"
#include "deps/cef3/include/cef_browser.h"
#include "deps/cef3/include/cef_command_line.h"
#include "deps/cef3/include/views/cef_browser_view.h"
#include "deps/cef3/include/views/cef_window.h"
#include "deps/cef3/include/wrapper/cef_helpers.h"

#include <xapian.h>

namespace {

// When using the Views framework this object provides the delegate
// implementation for the CefWindow that hosts the Views-based browser.
class SimpleWindowDelegate : public CefWindowDelegate {
 public:
  explicit SimpleWindowDelegate(CefRefPtr<CefBrowserView> browser_view)
      : browser_view_(browser_view) {
  }

  void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE {
    // Add the browser view and show the window.
    window->AddChildView(browser_view_);
    window->Show();

    // Give keyboard focus to the browser view.
    browser_view_->RequestFocus();
  }

  void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE {
    browser_view_ = NULL;
  }

  bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE {
    // Allow the window to close if the browser says it's OK.
    CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
    if (browser)
      return browser->GetHost()->TryCloseBrowser();
    return true;
  }

 private:
  CefRefPtr<CefBrowserView> browser_view_;

  IMPLEMENT_REFCOUNTING(SimpleWindowDelegate);
  DISALLOW_COPY_AND_ASSIGN(SimpleWindowDelegate);
};

}  // namespace

SimpleApp::SimpleApp() {
}

void SimpleApp::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();

  CefRefPtr<CefCommandLine> command_line =
      CefCommandLine::GetGlobalCommandLine();

#if defined(OS_WIN) || defined(OS_LINUX)
  // Create the browser using the Views framework if "--use-views" is specified
  // via the command-line. Otherwise, create the browser using the native
  // platform framework. The Views framework is currently only supported on
  // Windows and Linux.
  const bool use_views = command_line->HasSwitch("use-views");
#else
  const bool use_views = false;
#endif

  // SimpleHandler implements browser-level callbacks.
  CefRefPtr<SimpleHandler> handler(new SimpleHandler(use_views));

  // Specify CEF browser settings here.
  CefBrowserSettings browser_settings;

  TCHAR *path = new TCHAR[MAX_PATH];
  ZeroMemory(path, MAX_PATH);
  // path == "d:\Project\Test\MFC\MFC\debug"
  GetCurrentDirectory(MAX_PATH, path);

  std::string url(path);
  url += "//site//index.html";
    
  if (use_views) {
    // Create the BrowserView.
    CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
        handler, url, browser_settings, NULL, NULL);

    // Create the Window. It will show itself after creation.
    CefWindow::CreateTopLevelWindow(new SimpleWindowDelegate(browser_view));
  } else {
    // Information used when creating the native window.
    CefWindowInfo window_info;

#if defined(OS_WIN)
    // On Windows we need to specify certain flags that will be passed to
    // CreateWindowEx().
    window_info.SetAsPopup(NULL, "cefsimple");
#endif

    // Create the first browser window.
    CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,
                                  NULL);
  }
}

void SimpleApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	CefRefPtr<CefV8Value> object = context->GetGlobal();

	CefRefPtr<CefV8Handler> myV8handle = this;
	CefRefPtr<CefV8Value> myFun = CefV8Value::CreateFunction(L"adjSearch", myV8handle);
	object->SetValue(L"adjSearch", myFun, V8_PROPERTY_ATTRIBUTE_NONE);
}

bool SimpleApp::Execute(const CefString & name, CefRefPtr<CefV8Value> object, const CefV8ValueList & arguments, CefRefPtr<CefV8Value>& retval, CefString & exception)
{
	if (name.compare(L"adjSearch") == 0)
	{
		if (arguments.size() == 1 && arguments[0]->IsString())
		{
			CefRefPtr<CefV8Value> arg0 = arguments[0].get();

			std::string src = arg0->GetStringValue();

			std::list<std::string> dest;
			QueryData(arg0->GetStringValue(), dest);

			CefRefPtr<CefV8Value> arry = CefV8Value::CreateArray(dest.size());
			std::list<std::string>::iterator iter = dest.begin();
			for (int i = 0; iter != dest.end(), i < dest.size(); iter++, i++)
			{
				arry->SetValue(i, CefV8Value::CreateString(*iter));
			}
			retval = arry;
		}
		return true;
	}
	return false;
}

void SimpleApp::QueryData(std::string src, std::list<std::string>& dest)
{
	// Open the database for searching.
	Xapian::Database db("./index_data");

	// Start an enquire session.
	Xapian::Enquire enquire(db);

	// Parse the query string to produce a Xapian::Query object.
	Xapian::QueryParser qp;
	//Xapian::Stem stemmer("english");
	//qp.set_stemmer(stemmer);
	qp.set_database(db);
	//qp.set_stemming_strategy(Xapian::QueryParser::STEM_SOME);
	Xapian::Query query = qp.parse_scws_query(src);

	// Find the top 10 results for the query.
	enquire.set_query(query);
	Xapian::MSet matches = enquire.get_mset(0, 5);

	for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i) {
		dest.push_back(i.get_document().get_data());
	}
}
