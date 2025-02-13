/* global Backbone, jQuery */

window.app = window.app || {}; // 确保全局变量 app 存在，如果不存在则创建一个空对象

(function _initScanSetupModel($) {
  'use strict';

  // 定义一个 Backbone 模型，用于处理扫描设置的逻辑
  window.app.ScanSetup = Backbone.Model.extend({
    url: '/mmsp/scanSetup',  // 定义模型的 RESTful API 的 URL 路径

    // 自定义 sync 方法，处理不同的 HTTP 请求方法 (read, create, update, delete)
    sync: function _doSync(method, model, options) {
      var jXHR = null;  // 定义一个变量来存储异步请求的对象

      switch (method) {
        case 'read':
          // 读取数据时，分多个请求获取不同的参数数据，避免一次性获取太多数据导致响应过大
          jXHR = $.when(
            window.app.ApiProxy.ajax({
              url: this.url + '/startChannel',  // 获取扫描起始通道数据
            }),
            window.app.ApiProxy.ajax({
              url: this.url + '/stopChannel',  // 获取扫描停止通道数据
            }),
            window.app.ApiProxy.ajax({
              url: this.url + '/scanCount',  // 获取扫描次数数据
            }),
            window.app.ApiProxy.ajax({
              url: this.url + '/scanInterval',  // 获取扫描间隔数据
            }),
            window.app.ApiProxy.ajax({
              url: this.url + '/leakCheckMass',  // 获取漏检质量数据
            }),
            window.app.ApiProxy.ajax({
              url: this.url + '/scanTimeTotal',  // 获取扫描总时间数据
            }),
            window.app.ApiProxy.ajax({
             url: this.url + '/captureFileStatus',  // 获取文件状态数据
            })
          ).done(function _gotValues(startChannelResp, stopChannelResp,
            scanCountResp, scanIntervalResp, leakCheckMassResp,
            scanTimeTotalResp, captureFileStatusResp) {
            // 请求完成后，更新模型的属性
            model.set({
              startChannel: startChannelResp[0],  // 设置扫描起始通道
              stopChannel: stopChannelResp[0],  // 设置扫描停止通道
              scanCount: scanCountResp[0],  // 设置扫描次数
              scanInterval: scanIntervalResp[0],  // 设置扫描间隔
              scanTimeTotal: scanTimeTotalResp[0],  // 设置扫描总时间
              leakCheckMass: leakCheckMassResp[0],  // 设置漏检质量
              captureFileStatus: captureFileStatusResp[0],  // 设置文件状态
            });
          });
          break;

        case 'create':
          // 创建请求时，准备要发送的查询参数
          var queryParams = {};
          var set = false;
          Object.keys(model.attributes).forEach(
            function _addToQueryParam(key) {
              if (key !== 'channels' && key in model.changed) {
                queryParams[key] = model.changed[key];  // 获取模型属性发生变化的字段
                set = true;
              }
            });
          if (set) {
            options.url = this.url;
            options.data = queryParams;
            jXHR = Backbone.sync(method, model, options);  // 使用 Backbone.sync 执行请求
          } else {
            jXHR = $.when();  // 如果没有属性变化，则不执行请求
          }
          break;

        case 'update':
        case 'delete':
        default:
          // 对于不支持的方法，抛出错误
          throw new Error('Unsupported sync method:' + method);
      }

      return jXHR;  // 返回异步请求对象
    },

    // 停止扫描操作
    stopScanning: function _stopScanning() {
      // 确保停止扫描时将这些命令发送出去，即使它们的值与当前值匹配
      this.set({
        scanStop: null,
        scanStart: null,
      });
      // 保存状态，发送停止扫描的命令
      return this.save({
        scanStop: 'Immediately',  // 立即停止扫描
      });
    },

    // 启动扫描操作
    startScanning: function _startScanning(startChannel, stopChannel,
      scanInterval, numOfScans) {
      // 确保开始扫描时将这些命令发送出去，即使它们的值与当前值匹配
      this.set({
        startChannel: 0,
        stopChannel: 0,
        scanStart: 0,
        scanInterval: -1,
        scanCount: -2,
      });
      // 保存扫描参数，并启动扫描
      return this.save({
        startChannel: startChannel,
        stopChannel: stopChannel,
        scanStart: 1,
        scanInterval: scanInterval ? scanInterval : 0,
        scanCount: numOfScans ? numOfScans : -1,
      });
    },

    /**
     * 启动漏检扫描
     * @param {Integer} startChannel - 启动扫描的起始通道
     * @param {Integer} stopChannel - 停止扫描的终止通道
     * @param {Integer} leakCheckMass - 漏检时的质量参数
     * @return {Promise} 返回保存操作的 Promise 对象
     */
    startLeakCheck:
      function startLeakCheck(startChannel, stopChannel, leakCheckMass) {
        // 确保设置命令参数发送出去，而非当前值
        this.set({
          startChannel: 0,
          stopChannel: 0,
          scanStart: 0,
          scanInterval: -1,
          scanCount: -2,
          leakCheckMass: -1,
        });
        // 保存漏检扫描的参数
        return this.save({
          startChannel: startChannel,
          stopChannel: stopChannel,
          scanStart: 1,
          scanInterval: 0,
          scanCount: -1,
          leakCheckMass: leakCheckMass,
        });
      },

    // 获取最新扫描的总时间
    getLatestScanTimeTotal:
      function _getLatestScanTimeTotal(startChannel, stopChannel) {
        var d = $.Deferred();  // 创建一个延迟对象
        this.set({
          startChannel: 0,
          stopChannel: 0,
        });
        // 发送请求更新扫描起始和停止通道
        window.app.ApiProxy.ajax({
          type: 'POST',
          url: this.url,
          data: {
            startChannel: startChannel,
            stopChannel: stopChannel,
          },
        }).done(function() {
          // 通道设置完成后，获取最新的扫描总时间
          return window.app.ApiProxy.ajax({
            url: this.url + '/scanTimeTotal',  // 获取最新的扫描总时间
          }).done(function _gotLatestScanTime(scanTimeTotal) {
            this.set({
              scanTimeTotal: scanTimeTotal,  // 设置扫描总时间
            });
            d.resolve(scanTimeTotal);  // 完成操作
          }.bind(this)).fail(d.reject);  // 失败时拒绝操作
        }.bind(this)).fail(d.reject);  // 请求失败时拒绝操作

        return d;  // 返回延迟对象，表示异步操作
      },
  });
})(jQuery);
