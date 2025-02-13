/* global jQuery, Handlebars, _, Backbone, Cookies */

// 确保 window.app 对象存在，如果没有就创建一个空对象
window.app = window.app || {};

(function _initScanBaseView($) {
  'use strict';

  /**
   * 视图的模板字符串。用于渲染视图的 HTML 模板。
   * @type {string}
   */
  var template = $('#scanBaseTemplate').html();

  /**
   * 用于存储扫描信息元素的 cookie 名称。
   * 这个 cookie 用于保存用户当前配置的扫描元素，以便用户在刷新页面时恢复。
   * @type {string}
   * @constant
   */
  var SCAN_INFO_ELEMENTS_COOKIE_NAME = 'mmsp-scan-info-elements';

  /**
   * 将扫描信息元素集合转换为 cookie 存储。相比直接以 JSON 格式存储，这种方式在存储较大数据时更为高效，
   * 因为存储 JSON 数据在大小超过 400KB 时会出现问题。
   * @param {window.app.ScanInfoElements} scanInfoElements - 当前配置的扫描信息元素集合。
   */
  function convertScanInfoElementToCookie(scanInfoElements) {
    // 存储 cookie 的数组，cookie 内容将由多个“面包屑”组成
    var cookieCrumbs = [];

    // 遍历扫描信息元素集合，每个元素都会被转换为一个面包屑字符串
    scanInfoElements.each(function toCookieCrumb(element) {
      var instance = element.get('instance');  // 获取扫描元素的实例
      // 如果没有实例，设置为 'null' 字符串，以便后续处理
      if (_.isUndefined(instance)) {
        instance = 'null';
      }
      // 创建一个面包屑字符串，包含类型、实例、颜色和符号，使用 | 进行分隔
      cookieCrumbs.push(element.get('type') + '|' +
        instance + '|' +
        element.get('color') + '|' +
        element.get('symbol'));
    });

    // 将面包屑字符串数组连接为一个以逗号分隔的字符串，并将其存储到 cookie 中
    Cookies.set(SCAN_INFO_ELEMENTS_COOKIE_NAME, cookieCrumbs.join(','));
  }

  /**
   * 将存储在 cookie 中的扫描信息元素转换为一个对象数组。
   * 该函数读取 cookie 中的值，并将每个存储的面包屑解析为对象，方便后续使用。
   * @return {object[]} 返回一个对象数组，每个对象代表一个扫描信息元素。
   */
  function convertCookieToScanInfoElementObjects() {
    // 获取存储在 cookie 中的面包屑字符串
    var cookieCrumbs = Cookies.get(SCAN_INFO_ELEMENTS_COOKIE_NAME);

    // 如果 cookie 存在，则解析它
    if (cookieCrumbs) {
      return _.map(cookieCrumbs.split(','), function(crumbs) {
        // 解析每个面包屑字符串，按 | 分割
        var crumbParts = crumbs.split('|');
        var instance = crumbParts[1];  // 获取实例部分
        // 如果实例是 'null' 字符串，说明该实例是空值
        if (instance === 'null') {
          instance = undefined;
        }
        // 返回一个对象，包含扫描信息元素的各个属性：类型、实例、颜色和符号
        return {
          type: crumbParts[0],  // 扫描元素的类型
          instance: instance,    // 扫描元素的实例
          color: crumbParts[2],  // 扫描元素的颜色
          symbol: crumbParts[3], // 扫描元素的符号
        };
      });
    } else {
      // 如果没有存储的 cookie，则返回一个空数组
      return [];
    }
  }
/**
 * 显示所有具有扫描功能的屏幕的视图。
 * @class ScanBaseView
 * @extends {window.app.BaseView}
 */
window.app.ScanBaseView = window.app.BaseView.extend({
  /**
   * 视图的 ID。
   * @type {string}
   */
  id: 'scan',

  /**
   * 视图的标签名。
   * @type {string}
   */
  tagName: 'div',

  /**
   * 视图容器的类名。
   * @type {string}
   */
  className: 'content-zone',

  /**
   * 视图使用的模板，利用 Handlebars 编译模板。
   */
  template: Handlebars.compile(template),

  /**
   * 初始化视图。
   * @param {object} options - 视图的选项。
   */
  initialize: function _initScanBaseView(options) {
    var self = this;

    // 扫描设置通道的集合，存储所有需要配置的扫描通道。
    var scanSetupChannels = new window.app.ScanSetupChannels();

    // 一个 Backbone 模型，用于指示扫描通道是否已完全加载。
    var ScanChannelSetupStatus = Backbone.Model.extend({
      defaults: {
        loadedScanSetupChannels: false, // 默认状态为未加载
      },
    });

    // 实例化扫描通道加载状态模型。
    var scanChannelSetupStatus = new ScanChannelSetupStatus();

    // 创建扫描设置模型，用来获取上次配置的扫描通道信息。
    var scanSetup = new window.app.ScanSetup();

    // 设置一个阈值，超过该值时会增加请求的间隔时间，防止请求过于频繁。
    var scanChannelThresholdValue = 25;

    // 从服务器获取扫描设置数据
    scanSetup.fetch().done(function _getScanSetup() {
      var fetchTimeouts = 0; // 设置请求延迟计数器
      var pendingFetches = []; // 存储每个请求的 deferred 对象
      var longLoadNotfications = null; // 用于长时间加载时的通知

      // 如果扫描通道的配置跨度大于阈值，给用户展示加载配置的提示
      if (scanSetup.get('stopChannel') - scanSetup.get('startChannel') >
        scanChannelThresholdValue) {
        longLoadNotfications = new window.app.Notification({
          level: 'WARN', // 警告级别
          text: $.i18n('mmsp-scan-loading-configuration'), // 提示文本，告知用户正在加载配置
        });
        options.Notifications.add(longLoadNotfications); // 显示通知
      }

      // 遍历扫描设置中的每个通道，依次获取通道的数据
      for (var i = scanSetup.get('startChannel');
        i <= scanSetup.get('stopChannel');
        i++) {
        var channel = new window.app.ScanSetupChannel({
          id: i, // 设置通道的 ID
        });

        // 封装每个通道的请求，使用延时加载
        var d = (function fetchChannel(channel, idx) {
          var d = $.Deferred(); // 创建 deferred 对象，用于异步控制
          setTimeout(function() {
            // 发起通道的获取请求
            channel.fetch().done(function() {
              d.resolve(); // 请求成功，标记完成
            }).fail(function() {
              d.reject(); // 请求失败，标记失败
            });
          }, fetchTimeouts); // 每次请求间隔延迟
          return d;
        })(channel, i);

        // 将当前请求的 deferred 对象添加到待处理列表
        pendingFetches.push(d);
        // 将当前通道添加到扫描设置通道的集合中
        scanSetupChannels.add(channel);

        // 如果已经发起的请求超过了阈值，增加 250 毫秒的延迟，以减缓请求速率
        if (i % scanChannelThresholdValue === 0) {
          fetchTimeouts += 250;
        }
      }

      // 使用 $.when 等待所有请求完成
      $.when.apply($, pendingFetches).done(function() {
        // 如果有长时间加载的通知，加载完成后移除该通知
        if (longLoadNotfications) {
          options.Notifications.remove(longLoadNotfications);
        }

        // 所有通道加载完成，继续初始化其他部分
        self._finishInit();
        
        // 更新扫描通道加载状态为已完成
        scanChannelSetupStatus.set({
          loadedScanSetupChannels: true,
        });
      }).fail(function _failedToRetrieveChannels() {
        // 如果通道获取失败，显示错误通知
        options.Notifications.add(new window.app.Notification({
          level: 'ERROR', // 错误级别
          text: $.i18n('mmsp-scan-error-could-not-retrieve-channels'), // 提示用户获取通道失败
        }));
      });
    });

// The overall scan info to tell us if we are
// scanning
this.ScanInfo = new window.app.ScanInfo();  // 创建一个扫描信息对象，负责管理当前的扫描状态
this.ScanInfo.fetch();  // 获取扫描信息，判断当前是否正在进行扫描

// Poll to make sure we know if we are scanning
// 定时器，每隔1500毫秒定时获取扫描信息，以确保我们始终了解是否正在进行扫描
this.scanInfoTimerId = setInterval(function _updateScanInfo() {
  this.ScanInfo.fetch();  // 定期获取扫描信息
}.bind(this), 1500);  // 使用bind确保上下文为当前视图对象

// Measurement scan tracking
// 测量扫描追踪对象，负责存储和管理扫描测量数据
this.measurementScans = new window.app.MeasurementBinaryScans(
  [],  // 初始为空数组，表示没有任何扫描数据
  {},  // 默认配置为空对象
  scanSetupChannels  // 使用扫描配置通道（从scanSetup获取的配置）
);

// Subviews
// 根据当前模式加载不同的子视图
if (this.model.get('mode') === 'monitor') {  // 如果当前模式为'monitor'（监视模式）
  // 监视模式下的子视图
  var scanInfoElements = new window.app.ScanInfoElements();  // 创建一个扫描信息元素集合
  var ionSource = new window.app.IonSource();  // 创建一个离子源对象
  var measurement = new window.app.Measurement();  // 创建一个测量对象

  // We want to pull up the last saved display elements from the
  // cookie.
  // 从cookie中加载最后保存的显示元素（扫描信息元素）
  var elements = convertCookieToScanInfoElementObjects();  // 转换cookie数据为扫描信息元素对象
  if (elements) {  // 如果cookie中有元素
    _.each(elements, function add(element) {
      scanInfoElements.add(new window.app.ScanInfoElement(element));  // 将每个元素添加到scanInfoElements集合中
    });
  }
  
  // 保存扫描信息元素到cookie，当元素变化时触发
  scanInfoElements.on('add remove change', function saveToCookie() {
    convertScanInfoElementToCookie(scanInfoElements);  // 将更新后的元素集合保存到cookie
  }, this);

  // 创建子视图
  this.subviews = [
    new window.app.MonitorSetupView({
      ScanInfo: this.ScanInfo,
      ScanInfoElements: scanInfoElements,
      ScanSetup: scanSetup,
      ScanSetupChannels: scanSetupChannels,
      ScanChannelSetupStatus: scanChannelSetupStatus,
      MeasurementScans: this.measurementScans,
      Notifications: options.Notifications,
    }),
    new window.app.MonitorGraphView({
      ScanInfo: this.ScanInfo,
      ScanSetup: scanSetup,
      ScanSetupChannels: scanSetupChannels,
      ScanChannelSetupStatus: scanChannelSetupStatus,
      Measurement: measurement,
      MeasurementScans: this.measurementScans,
      ScanInfoElements: scanInfoElements,
      IonSource: ionSource,
    }),
  ];

  // 触发事件，在子视图之间传递unit选择
  this.subviews[0].on("optionSelected",function(e){
    this.subviews[1].trigger("optionSelected");  // 当第一个子视图选择单位时，触发第二个子视图的相应事件
  },this
  );
} else if (this.model.get('mode') === 'leak-check') {  // 如果当前模式为'leak-check'（泄漏检查模式）
  // 客户端模型用于在子视图之间共享阈值等数据
  var soundOnCookie = Cookies.get('soundOn');  // 从cookie中获取声音设置
  var ThresholdModel = Backbone.Model.extend({
    defaults: {
      alarm: 0,  // 默认报警阈值为0
      warning: 0,  // 默认警告阈值为0
      soundOn: soundOnCookie !== "false",  // 根据cookie设置是否启用声音
    },
  });

// 创建一个阈值模型，用于存储和管理报警和警告的阈值设置
var thresholdModel = new ThresholdModel();

// IonSource对象在两个视图之间共享，方便它们都能触发fetch操作来获取数据
var ionSource = new window.app.IonSource();
ionSource.fetch();  // 获取离子源的数据

// 根据当前模式加载不同的子视图
if (this.model.get('mode') === 'leak-check') {  // 如果当前模式为'leak-check'（泄漏检查模式）
  // 创建子视图
  this.subviews = [
    // 'LeakCheckSetupView' 子视图负责显示设置界面，并使用thresholdModel和其他相关的扫描数据
    new window.app.LeakCheckSetupView({
      model: thresholdModel,  // 将阈值模型传递给子视图
      ScanInfo: this.ScanInfo,  // 将扫描信息传递给子视图
      ScanSetup: scanSetup,  // 将扫描设置传递给子视图
      ScanSetupChannels: scanSetupChannels,  // 扫描通道设置
      ScanChannelSetupStatus: scanChannelSetupStatus,  // 扫描通道的状态
      Notifications: options.Notifications,  // 通知功能
      IonSource: ionSource,  // 离子源对象
    }),
    // 'LeakCheckGraphView' 子视图负责显示泄漏检查的图形视图
    new window.app.LeakCheckGraphView({
      model: thresholdModel,  // 将阈值模型传递给子视图
      MeasurementScans: this.measurementScans,  // 测量扫描数据
      ScanInfo: this.ScanInfo,  // 扫描信息
      IonSource: ionSource,  // 离子源对象
    }),
  ];
} else if (this.model.get('mode') === 'tune') {  // 如果当前模式为'tune'（调谐模式）
  // 创建调谐模式下的子视图
  this.subviews = [
    new window.app.TuneView({
      ScanInfo: this.ScanInfo,  // 扫描信息
      ScanSetup: scanSetup,  // 扫描设置
      ScanSetupChannels: scanSetupChannels,  // 扫描通道设置
      ScanChannelSetupStatus: scanChannelSetupStatus,  // 扫描通道状态
      MeasurementScans: this.measurementScans,  // 测量扫描数据
      Notifications: options.Notifications,  // 通知功能
    }),
  ];
}

/**
 * 完成视图的初始化
 */
_finishInit: function _finish() {
  // 监听扫描状态的变化
  this.ScanInfo.on(
    'change:scanning',  // 当扫描状态发生变化时触发
    function _scanningChange(model, scanning) {
      // 如果我们之前没有扫描，但现在开始扫描
      // 我们想清空所有旧的测量扫描，并重新添加当前的扫描数据
      if (model.previousAttributes().scanning === false && scanning) {
        this.measurementScans.reset([]);  // 清空旧的测量扫描数据
      }
      if (scanning) {  // 如果正在进行扫描
        // 如果在50ms以内重新开始了扫描，清除旧的定时器
        clearTimeout(this.lastChanceTimerId);
        this.lastChanceTimerId = null;
        this.restartPolling();  // 重新开始轮询
      } else {  // 如果停止扫描
        // 可能在我们收到最终轮询结果之前扫描就停止了
        // 所以我们要等到最后一次轮询结果出来
        this.lastChanceTimerId = setTimeout(function() {
          this.measurementScans.fetch().done(function() {
            // 如果定时器被触发，但fetch请求花费了较长时间
            // 需要确保重启操作没有被触发
            if (this.lastChanceTimerId !== null) {
              // 停止轮询
              this.stopPolling();
              this.lastChanceTimerId = null;  // 清除定时器ID
            }
          }.bind(this));
        }.bind(this), 50);  // 延迟50ms进行轮询
      }
    }, this);


// 如果已经有扫描信息且正在扫描
// 我们希望启动轮询获取最新的扫描数据
if (this.ScanInfo.get('scanning')) {
  this.restartPolling();  // 如果正在扫描，重新启动轮询
} else {
  // 如果没有扫描，检查是否有旧的扫描数据需要显示
  this.measurementScans.fetch();  // 获取旧的扫描数据
},

/**
 * 停止轮询以获取最新的扫描数据。
 */
stopPolling: function _stopPolling() {
  window.clearInterval(this.currentScanTimerId);  // 清除当前扫描定时器
  this.currentScanTimerId = null;  // 将定时器ID设为null
},

/**
 * 重新开始轮询获取最新的扫描数据。
 */
restartPolling: function _restartPolling() {
  // 停止任何现有的轮询操作
  this.stopPolling();

  // TODO: 根据当前扫描计算一个更好的间隔时间，基于服务器上设置的总时间
  this.measurementScans.fetch();  // 获取当前扫描数据
  var intervalTime = 500;  // 设置轮询间隔时间为500ms
  this.currentScanTimerId = setInterval(function _updateLatestScan() {
    this.measurementScans.fetch();  // 每500ms获取一次最新的扫描数据
  }.bind(this), intervalTime);  // 使用setInterval创建定时器
},

/**
 * 移除所有子视图并清除所有定时器。
 */
remove: function _remove() {
  // 移除所有子视图
  if (this.subviews) {
    this.subviews.forEach(function _renderSubview(subview) {
      subview.remove();  // 对每个子视图调用remove方法，销毁它
    });
    this.subviews = null;  // 清空subviews数组
  }

  // 清除轮询定时器的ID
  window.clearInterval(this.scanInfoTimerId);
  window.clearInterval(this.currentScanTimerId);
  window.clearInterval(this.lastChanceTimerId);
  this.lastChanceTimerId = null;  // 清空lastChanceTimerId

  // 调用父类的remove方法，确保视图被完全清除
  window.app.BaseView.prototype.remove.apply(this, arguments);
},

/**
 * 渲染所有子视图。
 * 使用了防抖（debounce）来确保不会频繁渲染，避免性能问题。
 */
_render: _.debounce(function _dRender() {
  if (!this.renderRequested) {
    return;  // 如果没有请求渲染，则直接返回
  }

  var templateVars = {};  // 渲染模板所需的变量
  this.$el.html(this.template(templateVars));  // 渲染模板并插入到当前视图元素

  // 添加图形视图
  var $container = this.$el.find('.content-zone-contents');  // 查找容器元素
  $container.html('');  // 清空容器
  this.subviews.forEach(function _renderSubview(subview) {
    $container.append(subview.render().el);  // 渲染每个子视图并将其添加到容器中
  });
}, 50),  // 使用防抖，确保每50ms最多只渲染一次
