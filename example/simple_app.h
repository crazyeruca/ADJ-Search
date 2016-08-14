// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

#include <list>
#include "deps/cef3/include/cef_app.h"

// Implement application-level callbacks for the browser process.
class SimpleApp : public CefApp,
                  public CefBrowserProcessHandler,
				  public CefRenderProcessHandler,
                  public CefV8Handler {
 public:
  SimpleApp();

  // CefApp methods:
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
      OVERRIDE { return this; }

  // CefApp methods:
  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler()
	  OVERRIDE { return this; }

  // CefBrowserProcessHandler methods:
  virtual void OnContextInitialized() OVERRIDE;

  // º¯Êý×¢²á
  virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
	  CefRefPtr<CefFrame> frame,
	  CefRefPtr<CefV8Context> context) OVERRIDE;

  // º¯ÊýÖ´ÐÐ
  virtual bool Execute(const CefString& name,
	  CefRefPtr<CefV8Value> object,
	  const CefV8ValueList& arguments,
	  CefRefPtr<CefV8Value>& retval,
	  CefString& exception) OVERRIDE;

protected:
	void QueryData(std::string src, std::list<std::string>& dest);

 private:
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(SimpleApp);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
