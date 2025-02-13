/* global Backbone, jQuery, _ */

window.app = window.app || {};

(function _initScanSetupChannelModel($) {
  'use strict';

  window.app.ScanSetupChannel = Backbone.Model.extend({
    // 定义不同类型的通道
    ChannelType: {
      // 常规类型
      FIXED_NUMBER: 'FixedNumber',  // 固定数字通道
      TIME: 'Time',                 // 时间通道
      TIMESTAMP: 'Timestamp',       // 时间戳通道

      // 状态类型
      SYSTEM_STATUS: 'SystemStatus', // 系统状态通道

      // 测量类型
      BASELINE: 'Baseline',          // 基线测量通道
      TPQUAD: 'TPQuad',              // 四分之一 TP 测量通道
      TOTAL_PRESSURE: 'TotalPressure', // 总压力测量通道

      // 质量类型
      SINGLE: 'Single',              // 单一质量通道
      SWEEP: 'Sweep',                // 扫描质量通道

      // 诊断类型
      PRESSURE: 'TotalPressureDetectorCurrent',  // 总压力探测电流
      EMISSION_CURRENT: 'EmissionCurrent',        // 发射电流
      ANODE_POTENTIAL: 'AnodePotential',          // 阴极电势
      ELECTRON_ENERGY: 'ElectronEnergy',          // 电子能量
      FOCUS_POTENTIAL: 'FocusPotential',          // 聚焦电势
      FILAMENT_CURRENT: 'FilamentCurrent',        // 灯丝电流

      // I/O 类型
      DIGITAL_INPUT: 'DigitalInput',        // 数字输入
      DIGITAL_OUTPUT: 'DigitalOutput',      // 数字输出
      ANALOG_INPUT_0: 'AnalogInput0V',      // 模拟输入0V
      ANALOG_INPUT_1: 'AnalogInput1V',      // 模拟输入1V
      ANALOG_INPUT_2: 'AnalogInput2V',      // 模拟输入2V
      ANALOG_INPUT_3: 'AnalogInput3V',      // 模拟输入3V
      ANALOG_INPUT_4: 'AnalogInput4V',      // 模拟输入4V
      ANALOG_INPUT_5: 'AnalogInput5V',      // 模拟输入5V
      ANALOG_INPUT_6: 'AnalogInput6V',      // 模拟输入6V
      ANALOG_INPUT_7: 'AnalogInput7V',      // 模拟输入7V
      ANALOG_INPUT_8: 'AnalogInput8V',      // 模拟输入8V
      ANALOG_INPUT_9: 'AnalogInput9V',      // 模拟输入9V
      ANALOG_OUTPUT_0: 'AnalogOutput0V',    // 模拟输出0V
      ANALOG_OUTPUT_1: 'AnalogOutput1V',    // 模拟输出1V
      ANALOG_OUTPUT_2: 'AnalogOutput2V',    // 模拟输出2V
      ANALOG_OUTPUT_3: 'AnalogOutput3V',    // 模拟输出3V
      ANALOG_OUTPUT_4: 'AnalogOutput4V',    // 模拟输出4V
      ANALOG_OUTPUT_5: 'AnalogOutput5V',    // 模拟输出5V
      ANALOG_OUTPUT_6: 'AnalogOutput6V',    // 模拟输出6V
      ANALOG_OUTPUT_7: 'AnalogOutput7V',    // 模拟输出7V
    },

    // 定义请求 URL 的根路径
    urlRoot: '/mmsp/scanSetup/channels',

    // 自定义同步方法，决定模型如何与后台通信
    sync: function _doSync(method, model, options) {
      var jXHR = $.Deferred(); // 使用 Deferred 对象管理异步操作

      switch (method) {
        case 'read':  // 当调用 read 方法时执行
          // 使用 ApiProxy 发送 AJAX 请求，获取通道数据
          window.app.ApiProxy.ajax({
            url: this.url(),
            // 接收到响应后进行处理
            complete: function(resp, result) {
              var data = resp.responseJSON.data;
              // 检查返回的数据是数组还是对象，并设置模型的数据
              if (_.isArray(data)) {
                model.set(data[0]);
              } else {
                model.set(data);
              }
              jXHR.resolve(); // 操作完成，调用 resolve
            },
          });
          break;

        case 'update':  // 当调用 update 方法时执行
        case 'create':  // 当调用 create 方法时执行
          var update = false; // 标记是否需要更新
          options.data = {};  // 初始化请求数据

          // 如果通道没有 channelType，则需要设置 channelMode
          if (!model.get('channelType')) {
            options.data.channelMode = model.get('channelMode');
            update = true;
          }
          // 遍历模型的变化，更新相应的属性
          for (var prop in model.changed) {
            options.data[prop] = model.changed[prop];
            update = true;
          }

          // 如果有数据需要更新，发送 PUT 请求
          if (update) {
            window.app.ApiProxy.ajax({
              url: this.url(),
              type: 'PUT',
              data: options.data,
              success: function(data) {
                // 如果返回的数据是数组，取第一个元素
                if (_.isArray(data)) {
                  data = data[0];
                }
                // 删除不需要的字段，并更新模型数据
                delete data['@id'];
                model.set(data);
                jXHR.resolve(); // 操作完成，调用 resolve
              },
              error: function() {
                jXHR.reject(); // 出现错误时，调用 reject
              },
            });
          } else {
            jXHR.resolve(); // 如果没有需要更新的数据，直接完成
          }
          break;

        case 'delete':  // 删除操作未实现，抛出错误
        default:
          throw new Error('Unsupported sync method:' + method);
      }

      return jXHR; // 返回 Deferred 对象，以便后续链式调用
    },
  });
})(jQuery);
