import React from 'react';
import ReactDOM from 'react-dom';

import 'antd/dist/antd.css';
import { Row, Col, Icon, Input, Button, Select } from 'antd';

import classNames from 'classnames';

import { observable, computed } from 'mobx';
import { observer } from 'mobx-react';

const Option = Select.Option;

const appStore = observable({
  placeholder: '请输入...',
  focus: false,
  value: '',
  data: []
});

appStore.handleChange = function(value) {
  this.value = value;
  this.data = [];

  const res = window.adjSearch(this.value);
  this.data = res.map( (val, index) => {
    return {value: index, text: val};
  });
}

appStore.callback = function(res) {
  //alert(res);
  //this.data = res.map( (val, index) => {
  //  return {value: index, text: val};
  //});
  this.data.push({value:1, text: "asad"});
  this.data.push({value:2, text: "afd"});
}

appStore.handleFocusBlur = function(isFocus) {
  this.focus = isFocus;
}


class App extends React.Component {
  constructor(props) {
    super(props);

    this.handleChange = this.handleChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
    this.handleFocusBlur = this.handleFocusBlur.bind(this);
  }

  handleChange(value) {
    this.props.store.handleChange(value);
  }

  handleSubmit() {
  }

  handleFocusBlur(e) {
    this.props.store.handleFocusBlur(e.target === document.activeElement);
  }

  render() {
    const btnCls = classNames({
      'ant-search-btn': true,
      'ant-search-btn-noempty': !!this.props.store.value.trim()
    });
    const searchCls = classNames({
      'ant-search-input': true,
      'ant-search-input-focus': this.props.store.focus
    });
    const options = this.props.store.data.map(d => <Option key={d.value}>{d.text}</Option>);
    return (
      <div>
        <Row>
          <Col span={6} >
            <div style={{marginTop: 100, marginLeft: 100}} >
              <p style={{fontSize:18}}>构建索引的原文:<br/></p>
              <p style={{fontSize:15}}>
              1、我是一个芒果<br/>
              2、我是一个香蕉<br/>
              3、我爱祖国天安门<br/>
              4、天安门上太阳照<br/>
              5、指引我们向前进<br/>
              </p>
            </div>
          </Col>
          <Col span={12} >
            <div style={{marginTop:50, textAlign:'center', fontSize:20}} >基于xapian和scws的实现的ADJ检索功能</div>
            <div style={{marginTop:100}} />
            <div className="ant-search-input-wrapper" style={this.props.style}>
              <Input.Group className={searchCls}>
                <Select
                  combobox
                  value={this.props.store.value}
                  placeholder={this.props.store.placeholder}
                  notFoundContent=""
                  defaultActiveFirstOption={false}
                  showArrow={false}
                  filterOption={false}
                  onChange={this.handleChange}
                  onFocus={this.handleFocusBlur}
                  onBlur={this.handleFocusBlur}
                >
                {options}
                </Select>
                <div className="ant-input-group-wrap">
                  <Button className={btnCls} onClick={this.handleSubmit}>
                    <Icon type="search" />
                  </Button>
                </div>
              </Input.Group>
            </div>            
          </Col>
          <Col span={6} />
        </Row>
      </div>
    );
  }
}
const ObservableApp = observer(App);


ReactDOM.render(<ObservableApp store={appStore}  style={{ width: 300, marginLeft:100 }} />, document.body);