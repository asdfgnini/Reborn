/* global jQuery, _, Highcharts, Handlebars, Backbone */
(function _initMonitorGraphView($) {
  'use strict';

  /**
   * The template to show on hover.
   * @type {string}
   */
  var hoverTemplate =
    Handlebars.compile($('#monitorGraphHoverTemplate').html());
  /**
   * The id of the series displaying single masses.
   * @type {string}
   * @constant
   */
  var SINGLE_MASS_SERIES_ID = 'single_mass_count';
  /**
   * The colors for scans that are overlayed from the
   * time graph onto the scan graph.
   * @type {string[]}
   * @constant
   */
  var SCAN_COLORS = Highcharts.theme.colors.slice(2, 7);
  /**
   * The prefix used on the id's for the series in a graph.
   * @type {string}
   * @constant
   */
  var CHANNEL_SERIES_PREFIX = 'CHANNEL_';
  /**
   * The name of the current scan. This applies to multiple
   * series but is our way of grouping them for easy
   * identification.
   * @type {string}
   * @constant
   */
  var CURRENT_SCAN_NAME = 'current';
  /**
   * The minimum amount of time for a total scan that is used
   * to show the current scan.
   * @type {number}
   * @constant
   */
  var CURRENT_SCAN_MIN_TIME = 1000;
  /**
   * The redraw rate of the graphs.
   * @type {number}
   * @constant
   */
  var REDRAW_RATE = 333;
  /**
   * The maximum number of scans to track.
   * @type {number}
   * @constant
   */
  var MAX_NUM_OF_SCANS = 100;
  /**
   * The list of symbols to use on this graph. Pulled from
   * Highcharts.SVGRenderer.prototype.symbols.
   * NOTE: Needs to be the same length as the scan colors!
   * @type {string[]}
   * @constant
   */
  var SYMBOLS = ['circle', 'square', 'triangle', 'triangle-down', 'diamond'];
  /**
   * The series id for displaying data in log form.
   * @type {string}
   * @constant
   */
  var LOG_DISPLAY_ID = 'logDisplay';
  /**
   * The series id for displaying data in linear form.
   * @type {string}
   * @constant
   */
  var LINEAR_DISPLAY_ID = 'linearDisplay';
  /**
   * The TIC series name
   * @type {string}
   * @constant
   */
  var TIC_SERIES_NAME = 'TIC';
  /**
   * Returns the series id for the ScanInfoElement
   * @param {string|Number} instance - The instance of the element.
   * @param {string} type - The type of the element.
   * @return {string} The generated id.
   */
  function getScanInfoSeriesId(instance, type) {
    var instanceId = _.isUndefined(instance) ? '' : instance;
    return 'allscansgraph_series_' + type +
      instanceId;
  }
  /**
   * Returns the Y Axis id for the ScanInfoElement
   * @param {window.app.ScanInfoElement} scanInfoElement
   * @return {string} The generated id.
   */
  function getScanInfoYAxisId(scanInfoElement) {
    if (scanInfoElement.get('type') === 'SingleMass') {
      return SINGLE_MASS_SERIES_ID;
    } else {
      return 'allscansgraph_yaxis_' + scanInfoElement.get('type');
    }
  }
  /**
   * Formats the value to an exponential with 2 decimal places
   * @param {Number} value - The value to format to exponential.
   * @return {string} The formatted value.
   */
  function formatExponentialValue(value) {
    if (String(value).indexOf('e') > -1 ||
      String(value).indexOf('.') > -1) {
      return value.toExponential(2);
    } else {
      return value;
    }
  }
  /**
   * Formats the value based on the scan info element type.
   * @param {window.app.ScanInfoElement} scanInfoElement - The
   * scan info element.
   * @param {Number} value  - The value to format.
   * @return {String} The formatted value.
   */
  function formatScanInfoElementValue(scanInfoElement, value) {
    var formattedValue;
    switch (scanInfoElement.get('type')) {
      case window.app.ScanSetupChannel.prototype.
        ChannelType.EMISSION_CURRENT:
        formattedValue = Handlebars.helpers.displayMicroAmps(value);
        break;
      case window.app.ScanSetupChannel.prototype.
        ChannelType.FILAMENT_CURRENT:
        formattedValue = Handlebars.helpers.displayAmps(value, 1000, false);
        break;
      case window.app.ScanSetupChannel.prototype.
        ChannelType.ANODE_POTENTIAL:
      case window.app.ScanSetupChannel.prototype.
        ChannelType.ELECTRON_ENERGY:
      case window.app.ScanSetupChannel.prototype.
        ChannelType.FOCUS_POTENTIAL:
        formattedValue = Handlebars.helpers.displayVoltage(value, 100, false);
        break;
      case window.app.ScanSetupChannel.prototype.
        ChannelType.TOTAL_PRESSURE:
        if (_.isNumber(value)) {
          formattedValue =
            value.toExponential(2) + 'Torr';
        } else {
          formattedValue = '';
        }
        break;
      case 'AnalogOutput':
      case 'AnalogInput':
        formattedValue = Handlebars.helpers.displayVoltage(value * 1000);
        break;
      default:
        formattedValue = formatExponentialValue(value);
        break;
    }
    return formattedValue;
  }
  /**
   * 返回用于显示属性的默认系列。
   * @return {Object[]} 包含默认系列的数组。
   */
  function getLinLogSeries() {
    // 当图表以对数模式显示时，要显示的名称。
    var linearUncheckedName = $.i18n('mmsp-monitor-linear');
    // 当图表以线性模式显示时，要显示的名称。
    var linearCheckedName = '\u2713 ' + $.i18n('mmsp-monitor-linear'); // \u2713 是一个复选标记符号
    // 当图表以对数模式显示时，要显示的名称。
    var logUncheckedName = $.i18n('mmsp-monitor-log');
    // 当图表以线性模式显示时，要显示的名称。
    var logCheckedName = '\u2713 ' + $.i18n('mmsp-monitor-log'); // \u2713 是一个复选标记符号

    // 切换线性显示模式的函数
    var toggleLinearDisplay = function toggleLinearDisplay(checked, scanChart) {
      var logSeries = scanChart.get(LOG_DISPLAY_ID);
      // 如果用户选择线性模式，则将图表切换到线性视图。
      if (checked) {
        logSeries.update({
          selected: false, // 更新系列的 selected 状态为未选中
          name: logUncheckedName, // 更新系列的名称为对数模式未选中时的名称
        }, false);
        logSeries.setVisible(false, false); // 设置对数模式的系列不可见
        scanChart.yAxis[0].update({
          type: 'linear', // 将图表的 y 轴类型更新为线性
        }, true);
      } else {
        // 否则将图表切换回对数视图。
        logSeries.update({
          selected: true, // 更新系列的 selected 状态为选中
          name: logCheckedName, // 更新系列的名称为对数模式选中时的名称
        }, false);
        logSeries.setVisible(true, false); // 设置对数模式的系列可见
        scanChart.yAxis[0].update({
          type: 'logarithmic', // 将图表的 y 轴类型更新为对数
        }, true);
      }
      // 根据用户期望立即重绘图表
      scanChart.redraw();
    };

    // 切换对数显示模式的函数
    var toggleLogDisplay = function toggleLogDisplay(checked, scanChart) {
      var linearSeries = scanChart.get(LINEAR_DISPLAY_ID);
      // 如果用户选择对数模式，则将图表切换到对数视图。
      if (checked) {
        linearSeries.update({
          selected: false, // 更新系列的 selected 状态为未选中
          name: linearUncheckedName, // 更新系列的名称为线性模式未选中时的名称
        }, false);
        linearSeries.setVisible(false, false); // 设置线性模式的系列不可见
        scanChart.yAxis[0].update({
          type: 'logarithmic', // 将图表的 y 轴类型更新为对数
        }, true);
      } else {
        // 否则将图表切换回线性视图。
        linearSeries.update({
          selected: true, // 更新系列的 selected 状态为选中
          name: linearCheckedName, // 更新系列的名称为线性模式选中时的名称
        }, false);
        linearSeries.setVisible(true, false); // 设置线性模式的系列可见
        scanChart.yAxis[0].update({
          type: 'linear', // 将图表的 y 轴类型更新为线性
        }, true);
      }
      // 根据用户期望立即重绘图表
      scanChart.redraw();
    };

    // 返回一个包含两个系列的对象数组，分别对应对数模式和线性模式
    return [{
      id: LOG_DISPLAY_ID, // 系列的唯一标识符，对应对数模式
      name: logUncheckedName, // 初始名称为对数模式未选中时的名称
      visible: false, // 初始状态下该系列不可见
      events: {
        legendItemClick: function(evt) {
          var scanChart = evt.target.chart; // 获取事件发生的图表对象
          var logSeries = scanChart.get(LOG_DISPLAY_ID); // 获取对数模式系列对象
          var checked = logSeries.selected !== true; // 计算系列的选中状态
          logSeries.update({
            selected: checked, // 更新系列的选中状态
            name: checked ? logCheckedName : logUncheckedName, // 根据选中状态更新系列名称
          }, false);
          toggleLogDisplay(checked, scanChart); // 调用切换对数显示模式的函数
        },
        checkboxClick: function(evt) {
          var scanChart = evt.target.chart; // 获取事件发生的图表对象
          var logSeries = scanChart.get(LOG_DISPLAY_ID); // 获取对数模式系列对象
          logSeries.setVisible(evt.checked, false); // 根据复选框状态设置系列可见性
          toggleLogDisplay(evt.checked, scanChart); // 调用切换对数显示模式的函数
        },
      },
    }, {
      id: LINEAR_DISPLAY_ID, // 系列的唯一标识符，对应线性模式
      name: linearCheckedName, // 初始名称为线性模式选中时的名称
      visible: true, // 初始状态下该系列可见
      selected: true, // 初始状态下该系列被选中
      events: {
        legendItemClick: function(evt) {
          var scanChart = evt.target.chart; // 获取事件发生的图表对象
          var linearSeries = scanChart.get(LINEAR_DISPLAY_ID); // 获取线性模式系列对象
          var checked = linearSeries.selected !== true; // 计算系列的选中状态
          linearSeries.update({
            selected: checked, // 更新系列的选中状态
            name: checked ? linearCheckedName : linearUncheckedName, // 根据选中状态更新系列名称
          }, false);
          toggleLinearDisplay(checked, scanChart); // 调用切换线性显示模式的函数
        },
        checkboxClick: function(evt) {
          var scanChart = evt.target.chart; // 获取事件发生的图表对象
          var linearSeries = scanChart.get(LINEAR_DISPLAY_ID); // 获取线性模式系列对象
          linearSeries.setVisible(evt.checked, false); // 根据复选框状态设置系列可见性
          toggleLinearDisplay(evt.checked, scanChart); // 调用切换线性显示模式的函数
        },
      },
    }];
  }
  /**
   * Formats the integer time value to a X:XX:XX string format.
   * @param {Number} value - The time value.
   * @return {string} The formatted time in a string.
   */
  function formatTimeValue(value) {
    var pad = function(num) {
      return ('0' + num).slice(-2);
    };

    // Convert to a hours:minutes:seconds string
    var valueBreakdown = value;
    var hours = Math.floor(valueBreakdown / 3.6e+6);
    valueBreakdown -= hours * 3.6e+6;
    var minutes = pad(Math.floor(valueBreakdown / 60000));
    valueBreakdown -= minutes * 60000;
    var seconds = pad(Math.floor(valueBreakdown / 1000));

    var timeLabel;
    if (hours >= 1) {
      timeLabel = hours + ':' + minutes + ':' + seconds;
    } else if (minutes >= 1) {
      timeLabel = '00:' + minutes + ':' + seconds;
    } else {
      timeLabel = '00:00:' + seconds;
    }
    return timeLabel;
  }

  // 创建 MonitorGraphView 视图
  window.app.MonitorGraphView = window.app.BaseView.extend({
    /**
     * 创建一个用于此视图的新容器。
     */
    tagName: 'div',
    /**
     * 将容器用此类名包裹。
     */
    className: 'scan-graph-container',
    /**
     * 初始化视图。
     * @param {object} options - 传递给此视图的选项。
     */
    initialize: function _initMonitorGraphView(options) {
      // 初始化属性值
      this.graphModel = new (Backbone.Model.extend({
        defaults: {
          scanColorIdx: 0, // 当前使用的扫描颜色索引
          measurementScanAvailable: false, // 表示是否有可用的测量扫描数据
          scanDataAvailable: false, // 表示是否有可用的扫描数据
          symbolIdxs: _.map(SCAN_COLORS, function initIndices(color, idx) {
            return idx; // 创建一个颜色索引数组
          }),
          pointsToRemove: [], // 需要从图表中移除的点
        },
      }))();
      this.Measurement = options.Measurement; // 测量数据
      this.IonSource = options.IonSource; // 离子源数据

      // 当 scanDataAvailable 改变时，重绘扫描图表。
      this.graphModel.on(
        'change:scanDataAvailable',
        this.redrawScanChart,
        this
      );

      // 当 measurementScanAvailable 改变时，重绘所有扫描图表。
      this.graphModel.on(
        'change:measurementScanAvailable',
        this.redrawAllScansGraph,
        this
      );

      // 当 unitType 改变时，重绘扫描图表和时间图表。
      this.graphModel.on(
        'change:unitType',
        this.unitChange,
        this
      );

      // 设置全局最小和最大值，用于防止快速缩放
      this.graphModel.set("globalMin", 0);
      this.graphModel.set("globalMax", 0);

      // 数据默认以 torr 为单位，设置默认的单位类型和乘数
      this.graphModel.set("unitType", "Torr");
      this.graphModel.set("unitMultiplier", 1);

      // 当从父级接收到事件时，设置存储在 cookies 中的选项为当前单位
      this.on("optionSelected", function(e) {
        if (Cookies.getJSON()["unitTypeSelectedValue"] == undefined) {
          this.graphModel.set("unitType", "Error-No-Cookie-Saved"); // 如果 cookies 中没有单位类型值，则设置错误信息
        } else {
          this.graphModel.set("unitType", Cookies.getJSON()["unitTypeSelectedValue"]["SavedOption"]); // 设置单位类型为 cookies 中存储的值
        }
      }, this);

      // 手动触发一次 optionSelected 事件以初始化乘数和类型
      this.trigger("optionSelected");

      // 扫描信息
      this.ScanInfo = options.ScanInfo;

      // 扫描设置
      this.ScanSetup = options.ScanSetup;

      // 包含扫描设置的通道
      this.ScanSetupChannels = options.ScanSetupChannels;

      // 要在时间图表中显示的附加元素
      this.ScanInfoElements = options.ScanInfoElements;

      // 当 ScanInfoElements 添加或移除元素时，更新时间图表
      this.ScanInfoElements.on(
        'add',
        this.addAdditionalDisplayElementToTimeGraph, // 注意这里函数名修正为 addAdditionalDisplayElementToTimeGraph
        this
      );
      this.ScanInfoElements.on(
        'remove',
        this.removeDisplayElementFromTimeGraph,
        this
      );

      // 当 ScanInfoElements 发生变化时，更新时间图表中的相应系列
      this.ScanInfoElements.on('change', function(scanInfoElement) {
        if (this.timeGraph) {
          // 获取旧属性以便移除已过时的系列
          var previousAttributes = scanInfoElement.previousAttributes();
          var id = getScanInfoSeriesId(
            previousAttributes.instance,
            previousAttributes.type
          );
          // 移除旧系列
          if (this.timeGraph.get(id)) {
            this.timeGraph.get(id).remove(false);
          }
          // 添加新的系列
          this.addAdditionalDisplayElementToTimeGraph(scanInfoElement); // 注意这里函数名修正为 addAdditionalDisplayElementToTimeGraph
        }
      }, this);

      // 扫描集合
      this.MeasurementScans = options.MeasurementScans;

      // 在渲染任何内容之前，确保我们有通道设置配置，以便正确渲染图表
      options.ScanChannelSetupStatus.once('change', function attachListeners() {
        // 监听 startChannel 和 stopChannel 的变化
        this.ScanSetup.on(
          'change:startChannel change:stopChannel',
          this.updateMonitorGraph,
          this
        );

        // 监听 ScanSetupChannels 的变化以正确显示内容
        this.ScanSetupChannels.on(
          'change add remove reset',
          this.updateMonitorGraph,
          this
        );

        // 定义一个包装函数，用于更方便地附加和分离扫描系列
        var wrappedUpdateScanSeries =
          function wrappedUpdateScanSeries(currentScan) {
            // 确保有当前扫描数据
            if (!currentScan) {
              return;
            }

            // 如果扫描号已改变，则更新扫描系列以确保完整图表
            if (!_.isUndefined(currentScan.changed.scannum)) {
              var scan = currentScan.collection.get(
                currentScan.get('scannum') - 1
              );
              if (scan) {
                this.updateScanSeries(
                  scan.get('measurements')
                );
              }
            }

            // 如果扫描速度较慢，更新当前扫描系列
            if (this.ScanSetup.get('scanTimeTotal') > CURRENT_SCAN_MIN_TIME) {
              this.updateScanSeries(currentScan.get('measurements'));
            }
          }.bind(this);

        // 在第一次添加扫描时，确保显示正确的列数
        this.MeasurementScans.once(
          'add',
          function recomputeColumns() {
            this.graphModel.set('needsToRecomputeColumns', true);
          },
          this
        );

        var displayLastAddTimer = null;

        // 监听扫描添加事件
        this.MeasurementScans.on(
          'add',
          function _mScanAdded(scan) {
            // 如果扫描数据尚未获取，则等待获取后再添加
            var measurements = scan.get('measurements');
            if (!measurements || !measurements.length) {
              scan.once('change', function gotMeasurements() {
                this.addScanToTimeGraph(scan);
              }, this);
            } else {
              this.addScanToTimeGraph(scan);
            }

            // 如果不处于扫描状态，添加最后一个扫描到扫描图表
            clearTimeout(displayLastAddTimer);
            displayLastAddTimer = setTimeout(function() {
              if (options.ScanInfo.get('scanning') === false) {
                this.updateScanSeries(scan.get('measurements'));
              }
            }.bind(this), 200);

            // 确保不会超出内存限制，移除最早的扫描
            if (this.MeasurementScans.length - 2 > MAX_NUM_OF_SCANS &&
              options.ScanInfo.get('scanning') === true) {
              var scanToRemove = this.MeasurementScans.at(1);
              this.MeasurementScans.remove(scanToRemove);
            }
          },
          this
        );

        // 如果当前扫描已经在集合中，则立即添加监听器
        if (this.MeasurementScans.get(0)) {
          this.currentScan = this.MeasurementScans.get(0);
          this.currentScan.on(
            'change',
            wrappedUpdateScanSeries,
            this
          );
          // 获取最新的扫描数据
          this.MeasurementScans.fetch().then(function() {
            // 如果没有扫描数据，则显示无数据信息
            if (!this.currentScan.get('measurements') ||
              this.currentScan.get('measurements').length === 0) {
              if (this.scanChart) {
                this.scanChart.hideNoData();
                this.scanChart.showNoData($.i18n('mmsp-scan-no-data-to-display'));
              }
            }
          }.bind(this));
        }

        // 当扫描集合重置时，处理逻辑
        this.MeasurementScans.on(
          'reset',
          function mScanReset() {
            // 获取当前扫描
            if (this.MeasurementScans.get(0)) {
              this.currentScan = this.MeasurementScans.get(0);
              this.currentScan.on(
                'change',
                wrappedUpdateScanSeries,
                this
              );
            }
            // 清除图表
            this.clearGraphs();

            // 显示正在计算基线或加载的信息
            if (this.scanChart) {
              this.scanChart.hideNoData();
              if (options.ScanInfo.get('scanning') === true) {
                this.scanChart.showNoData($.i18n('mmsp-monitor-calculating-baseline'));
              } else {
                this.scanChart.showNoData($.i18n('mmsp-scan-loading'));
              }
            }
            // 重新计算列大小
            this.graphModel.set('needsToRecomputeColumns', true);
          },
          this
        );

        // 使用现有数据更新监控图表
        this.updateMonitorGraph();
      }, this);
    },
  });
    },

    unitChange: function unitChange(){
      //divide by this to change units to original torr
      var divisor = this.graphModel.get("unitMultiplier");

      this.IonSource.fetch().then(function() {
        var Sensitivity = this.IonSource.get("ppSensitivityFactor");
        var FragFactor = this.IonSource.get("ppLinConst1");
        var IonizationProbability = this.IonSource.get("ppLinConst2");

        // If the values received are incorrect set them to known constants
        //If the values recieved are incorrect set them to known constants
        if(isNaN(Sensitivity) || Sensitivity == 0){
          Sensitivity = 1;
        }
        if(isNaN(FragFactor) || FragFactor == 0){
          FragFactor = 0.94;
        }
        if(isNaN(IonizationProbability) || IonizationProbability == 0){
          IonizationProbability = 1;
        }

        switch(this.graphModel.get("unitType")) {
          case "Amps":
            this.graphModel.set("unitMultiplier", 1);
            break;
          case "Torr":
            this.graphModel.set("unitMultiplier", 1/(Sensitivity*FragFactor*IonizationProbability));
            break;
          case "mB":
            this.graphModel.set("unitMultiplier", 1.333222/(Sensitivity*FragFactor*IonizationProbability));
            break;
          case "Pa":
            this.graphModel.set("unitMultiplier", 133.3222/(Sensitivity*FragFactor*IonizationProbability));
            break;
          case "PPM":
            this.graphModel.set("unitMultiplier", (1/(Sensitivity * FragFactor * IonizationProbability)) * 1000000);
            break;
          default:
            this.graphModel.set("unitType", $.i18n('mmsp-amps'));
            this.graphModel.set("unitMultiplier", 1);
        }
        var newMultiplier = this.graphModel.get("unitMultiplier");

        if(this.timeGraph){
          this.timeGraph.series.forEach(function(series){
            let dataArr = [];
            series.yData.forEach(function(data){
              dataArr.push(data*newMultiplier/divisor);
            })
            series.setData(dataArr,true,true,true);
          })
        }
        if(this.scanChart){
          this.scanChart.series.forEach(function(series)
          {
            let dataArr = [];
            series.yData.forEach(function(data){
              dataArr.push(data*newMultiplier/divisor);
            })
            series.setData(dataArr,true,true,true);
          })
          // redraw graph on unit change
          this.updateMonitorGraph();
          
          // update axis label to reflect current units
          this.scanChart.yAxis[0].setTitle({
            text: this.graphModel.get("unitType")
          });
        }

        this.clearGraphs();

        // reset extremes on unit change
        this.graphModel.set("globalMin", 0);
        this.graphModel.set("globalMax", 0);
      }.bind(this));      
    },
    /**
     * Clears all the series from all the charts.
     */
    clearGraphs: function clearGraphs() {
      // Clear scan chart
      if (this.scanChart) {
        var seriesToRemove = [];
        // Identify all the series that aren't current
        // and mark them for removal. For the current
        // series, clear out any existing data
        _.each(this.scanChart.series, function _determineIfRemove(series) {
          if (series.userOptions.name !== 'current' &&
            series.id !== LINEAR_DISPLAY_ID &&
            series.id === LOG_DISPLAY_ID) {
            seriesToRemove.push(series);
          } else {
            series.setData([], false);
          }
        });
        // Remove all the series
        _.each(seriesToRemove, function _remove(series) {
          series.remove(false);
        });
        // Hide bouncey bar
        if (this.scanChart.xAxis[0] &&
          this.scanChart.xAxis[0].plotLinesAndBands.length) {
          this.scanChart.xAxis[0].plotLinesAndBands[0].svgElem.hide();
        }
        // Set the flag to update
        this.graphModel.set('scanDataAvailable', true);
      }
      // Update the time graph
      if (this.timeGraph) {
        // Because of navigator bugs, we enable and disable it
        this.timeGraph.update({
          navigator: {
            enabled: false,
          },
        }, true);
        this.timeGraph.xAxis[0].setExtremes(0, 0, false);
        _.each(this.timeGraph.series, function remove(series) {
          series.setData([], false);
        });
        this.timeGraph.update({
          navigator: {
            enabled: true,
          },
        }, true);
        this.timeGraph.redraw();
      }
      // Reset indexes
      this.graphModel.set({
        'symbolIdxs': _.map(SCAN_COLORS, function initIndices(color, idx) {
          return idx;
        }),
        // Clear the tracking
        'lastPointIndex': 0,
        'lastSeriesUpdated': null,
      });
    },
    /**
     * Process the TIC and whole mass values from the measurements.
     * @param {app.MeasurementScan[]} measurements - The measurements
     * to process.
     * @return {object} The TIC and whole mass value.
     */
    processMeasurements: function processMeasurements(measurements) {
      // For displaying single mass values
      var wholeMassValues = {};
      // Total Ion Count
      var tic = 0;
      for (var i = 0; i < measurements.length; i++) {
        var mass = measurements[i].startMass;
        // Find the min of the measurements
        // Translate to 0 then apply that translation
        // factor to the rest of the points. Once there,
        // then summate them.
        _.each(measurements[i].data, function _sum(val, idx) {
          tic += Math.abs(val);
          if (idx % measurements[i].ppamu === 0) {
            wholeMassValues[mass] = val;
            mass++;
          }
        });
      }

      return {
        tic: tic,
        wholeMassValues: wholeMassValues,
      };
    },
    /**
     * Returns the datum for the scan info element from the
     * scan.
     * @param {app.ScanInfoElement} scanInfoElement - The info element.
     * @param {app.MeasurementScan} scan - The scan to get the datum from.
     * @param {object} wholeMassValues - Object mapping the whole mass values
     * to their values from the entire scan.
     * @return {number} The datum.
     */
    getScanInfoDatum:
      function getScanInfoDatum(scanInfoElement, scan, wholeMassValues) {
        var type = scanInfoElement.get('type');
        var instance;
        var datum;
        switch (type) {
          case 'SingleMass':
            // Find the integer mass from the scan
            instance = scanInfoElement.get('instance');
            datum = wholeMassValues[instance];
            break;
          case 'DigitalInput':
          case 'DigitalOutput':
            instance = scanInfoElement.get('instance');
            datum = ((scan.get(type) >> (instance - 1)) & 1);
            break;
          case 'AnalogInput':
          case 'AnalogOutput':
            instance = scanInfoElement.get('instance');
            var channelId = 'ANALOG_';
            if (type === 'AnalogInput') {
              channelId += 'INPUT_' + instance;
            } else {
              channelId += 'OUTPUT_' + instance;
            }
            datum = scan.get(window.app.ScanSetupChannel.prototype.
              ChannelType[channelId]);
            break;
          default:
            // This is for scalar values stored on the scan
            // like Total Pressure, Filament Current, etc.
            datum = scan.get(type);
            break;
        }
        return datum;
      },
    /**
     * Add a newly completed scan to the time graph.
     * @param {app.MeasurementScan} scan - The scan to add.
     */
    addScanToTimeGraph: function addScanToTimeGraph(scan) {
      // Figure out the TIC fist
      var measurements = scan.get('measurements');
      if (measurements) {
        var results = this.processMeasurements(measurements);
        var wholeMassValues = results.wholeMassValues;
        var tic = results.tic;
        tic *= this.graphModel.get("unitMultiplier");;
        var scanTime = scan.get(
          window.app.ScanSetupChannel.prototype.
            ChannelType.TIMESTAMP);
        var shift =
          this.ScanInfo.get('scanning') === true &&
          this.timeGraph.series[0].options.data.length > MAX_NUM_OF_SCANS;
        this.timeGraph.series[0].addPoint({
          x: scanTime,
          y: tic,
          id: scan.id,
        }, false, shift);

        // Add the data point for each additional scan display element
        var totalPressure = this.Measurement.get("totalPressure");
        this.ScanInfoElements.each(function(scanInfoElement) {
          var id = getScanInfoSeriesId(
            scanInfoElement.get('instance'),
            scanInfoElement.get('type'));
          var series = this.timeGraph.get(id);
          if (!series) {
            console.warn('Series does not exist!', id);
            return;
          }
          let scanData = this.getScanInfoDatum(scanInfoElement, scan, wholeMassValues);

          if(scanInfoElement.get('type') === "SingleMass") {
            scanData*=this.graphModel.get("unitMultiplier");;
            if(this.graphModel.get('unitType')=== "PPM") {
              scanData /= totalPressure;
            }
          }
          series.addPoint([
            scanTime,
            scanData,
          ], false, shift);
        }.bind(this));

        // Now set the flag to redraw the graph
        this.graphModel.set('measurementScanAvailable', true);
      }
    },
    /**
     * Adds an additional display element to the time graph.
     * @param {app.ScanInfoElement} scanInfoElement - The
     * element to add to the graph.
     */
    addAdditionalDisplayElementoTimeGraph:
      function addAdditionalDisplayElementoTimeGraph(scanInfoElement) {
        var yAxisId = getScanInfoYAxisId(scanInfoElement);
        if (!this.timeGraph.get(yAxisId)) {
          this.timeGraph.addAxis({
            id: yAxisId,
            visible: false,
            height: '35%',
          }, false, false);
        }
        var id = getScanInfoSeriesId(
          scanInfoElement.get('instance'),
          scanInfoElement.get('type'));
        // Don't add the series again if we already have it.
        if (!this.timeGraph.get(id)) {
          var data = [];
          this.MeasurementScans.each(function(scan) {
            // Don't include the current scan.
            if (scan.id === 0) {
              return;
            }
            // Figure out the TIC first
            var measurements = scan.get('measurements');
            if (measurements) {
              var results = this.processMeasurements(measurements);
              var wholeMassValues = results.wholeMassValues;
              var scanTime = scan.get(
                window.app.ScanSetupChannel.prototype.
                  ChannelType.TIMESTAMP);
              var ScanInfoDatum = this.getScanInfoDatum(scanInfoElement, scan, wholeMassValues);
              ScanInfoDatum*=this.graphModel.get("unitMultiplier");
              data.push([
                scanTime,
                ScanInfoDatum,
              ]);
            }
          }.bind(this));
          // Add it as a series with the data and immediately redraw
          // for user response
          this.timeGraph.addSeries({
            id: id,
            name: scanInfoElement.convertToTemplateVar().text,
            scanInfoElement: scanInfoElement,
            type: 'line',
            data: data,
            marker: {
              symbol: scanInfoElement.get('symbol'),
            },
            color: scanInfoElement.get('color'),
            yAxis: yAxisId,
            dataLabels: [{
              enabled: true,
              /**
               * Restricts the expoential to 2 decimal places
               * @return {string} THe string
               */
              formatter: function() {
                return formatScanInfoElementValue(
                  scanInfoElement,
                  this.y);
              },
            }],
          });
        }
      },
    /**
     * Removes the display element from the graph.
     * @param {app.ScanInfoElement} scanInfoElement - The element to remove.
     */
    removeDisplayElementFromTimeGraph:
      function removeDisplayElementFromTimeGraph(scanInfoElement) {
        var id = getScanInfoSeriesId(
          scanInfoElement.get('instance'),
          scanInfoElement.get('type'));
        var series = this.timeGraph.get(id);
        if (series) {
          series.remove();
        }
      },
    /**
     * Redraws the all scan graph at a controlled rate.
     */
    redrawAllScansGraph: _.debounce(function redrawAllScansGraph() {
      if (this.graphModel.get('measurementScanAvailable')) {
        this.timeGraph.redraw();
        this.graphModel.set({
          'measurementScanAvailable': false,
        }, {
          silent: true,
        });

        // The navigator doesn't update unless we set the extremes
        // at the right point when we want to start controlling
        // the number of columns we want to show and the first
        // point it not === 0
        if (this.timeGraph.series[0].data.length - 2 ===
          this.graphModel.get('numOfColumns')) {
          this.updateExtremes();
        }
      }
    }, REDRAW_RATE),
    /**
     * Renders the graphs.
     */
    _render: function render() {
      if (!this.renderRequested) {
        return;
      }
      // Destroys any existing graphs
      if (this.scanChart) {
        this.scanChart.destroy();
        this.scanChart = null;
      }
      if (this.timeGraph) {
        this.timeGraph.destroy();
        this.timeGraph = null;
      }

      var self = this;
      var $currentScanContainer =
        $('<div class="current-scan-graph"></div>').prependTo(this.$el);
      this.scanChart = new Highcharts.Chart({
        // 图表的基本设置
        chart: {
          // 渲染目标：指定图表将被渲染到的HTML元素
          renderTo: $currentScanContainer[0],
          // 缩放类型：允许沿x轴和y轴进行缩放
          zoomType: 'xy',
          // 重置缩放按钮的位置设置
          resetZoomButton: {
            position: {
              // 按钮在y轴上的位置
              y: 70,
            },
          },
          // 图表底部的间距
          spacingBottom: 45,
          // 图表底部的外边距
          marginBottom: 75,
        },
      
        // 语言设置
        lang: {
          // 没有数据时的显示文本
          noData: '',
          // 重置缩放的文字
          resetZoom: $.i18n('mmsp-reset-zoom'),
          // 重置缩放的提示文字
          resetZoomTitle: $.i18n('mmsp-reset-zoom'),
        },
      
        // 去除版权信息
        credits: {
          // 禁用版权信息显示
          enabled: false,
        },
      
        // x轴设置
        xAxis: {
          // x轴标题设置
          title: {
            // x轴标题文本，使用国际化函数获取
            text: $.i18n('mmsp-mass'),
          },
          // 绘图线设置
          plotLines: [{
            // 绘图线的值
            value: 0,
            // 绘图线的颜色
            color: '#AAA',
            // 绘图线的宽度
            width: 2,
            // 绘图线的样式（虚线）
            dashStyle: 'dash',
            // 绘图线的ID
            id: 'plot-line-1',
          }],
        },
      
        // y轴设置
        yAxis: {
          // y轴标题设置
          title: {
            // y轴标题文本，从graphModel模型中获取
            text: this.graphModel.get("unitType"),
          },
          // y轴标签格式化
          labels: {
            formatter: function(evt) {
              // 如果y轴是对数类型
              if (evt.axis.options.type === 'logarithmic') {
                // 获取y轴的极值
                var extremes = this.axis.getExtremes();
                var max;
                var min;
                // 如果用户设置了最小值和最大值
                if (_.isNumber(extremes.userMin) && _.isNumber(extremes.userMax)) {
                  max = extremes.userMax;
                  min = extremes.userMin;
                } else {
                  // 否则使用数据的最大值和最小值
                  max = extremes.dataMax;
                  min = extremes.dataMin;
                }
                // 如果是最后一个标签，显示最大值
                if (this.isLast) {
                  return formatExponentialValue(max);
                }
                // 如果是第一个标签，显示最小值
                else if (this.isFirst) {
                  return formatExponentialValue(min);
                }
                // 否则不显示标签
                else {
                  return '';
                }
              } else {
                // 如果不是对数类型，直接格式化当前值
                return formatExponentialValue(this.value);
              }
            },
          },
        },
      
        // 图表标题设置
        title: null,
      
        // 图例设置
        legend: {
          // 启用图例
          enabled: true,
          // 图例浮动显示
          floating: true,
          // 图例垂直排列
          layout: 'vertical',
          // 图例垂直对齐方式（顶部）
          verticalAlign: 'top',
          // 图例水平对齐方式（右侧）
          align: 'right',
          // 图例符号的内边距
          symbolPadding: 0,
          // 图例符号的宽度
          symbolWidth: 0,
          // 图例符号的半径
          symbolRadius: 0,
          // 图例距离顶部的距离
          y: 50,
        },
      
        // 提示框设置
        tooltip: {
          // 启用提示框
          enabled: true,
          // 提示框格式化函数
          formatter: function() {
            // 准备提示框模板变量
            var templateVars = {
              points: this.points,
            };
            // 如果没有多个数据点，则将单个数据点放入数组中
            if (!this.points) {
              templateVars.points = [this.point];
            }
            // 返回自定义的提示框内容
            return hoverTemplate(templateVars);
          },
        },
      
        // 数据系列设置
        series: getLinLogSeries(),
      });

      var $allScansContainer =
        $('<div class="all-scans-graph"></div>').appendTo(this.$el);
      this.timeGraph = new Highcharts.Chart({

        chart: {
          renderTo: $allScansContainer[0],
          borderRadius: 0,
          spacingBottom: 50,
          marginBottom: 75,
          spacingTop: 0,
          marginTop: 0,
          events: {
            /**
             * Maximizes the number of bars to show given the real estate.
             */
            redraw: function redraw() {
              if (self.graphModel.get('needsToRecomputeColumns') === true) {
                // Unset the flag
                self.graphModel.set('needsToRecomputeColumns', false);

                // Determine how many bars we can show given the
                // available real estate
                var columnWidth = 108;
                if (this.series[0] &&
                  this.series[0].points.length) {
                  columnWidth = this.series[0].points[0].pointWidth * 2;
                }
                // Update the model with the new value.
                self.graphModel.set(
                  'numOfColumns',
                  Math.floor(this.plotWidth / columnWidth));
                // Now update the extremes
                self.updateExtremes();
              }
            },
          },
        },

        title: null,

        tooltip: {
          enabled: true,
          formatter: function() {
            if (this.series.name === TIC_SERIES_NAME) {
              return false;
            } else if (this.series.userOptions.type === 'line') {
              // For all series minus the scatter plot markers
              var y = formatScanInfoElementValue(
                this.series.userOptions.scanInfoElement,
                this.y);
              return '<b>' + this.series.name + '</b>:' + y;
            }
          },
        },

        navigator: {
          enabled: true,
          handles: {
            enabled: false,
          },
          yAxis: {
            plotBands: [{
              from: Number.MIN_SAFE_INTEGER,
              to: Number.MAX_SAFE_INTEGER,
              // TODO : This is Linxon Specific
              color: '#E6E7E8',
            }],
          },
          xAxis: {
            labels: {
              formatter: function(value) {
                // This is to prevent it from showing 'Jan 1'
                if (this.value === 0) {
                  return '';
                } else {
                  return this.axis.defaultLabelFormatter.call(this);
                }
              },
            },
          },
        },

        legend: {
          enabled: false,
        },

        credits: {
          enabled: false,
        },

        series: [{
          type: 'column',
          id: TIC_SERIES_NAME,
          name: TIC_SERIES_NAME,
          color: Highcharts.theme.colors[1],
          cursor: 'pointer',
          dataLabels: {
            enabled: false,
          },
          events: {
            'click': function _showScanInGraph(evt) {
              // The name of the scan
              var id = evt.point.id;
              var options = evt.point.options;
              var measurementScan = self.MeasurementScans.get(id);
              var name = measurementScan.get('scannum');
              // If point is already locked, we should unlock it
              if (evt.point.options.className === 'locked') {
                // Remove the flag that it is locked
                options.className = '';
                evt.point.update({
                  options: options,
                  color: null,
                }, false);

                // Find the series to remove that were associated with
                // this point in time.
                var seriesToRemove = [];
                self.scanChart.series.forEach(
                  function _checkToRemove(series) {
                    if (series.name === name) {
                      seriesToRemove.push(series);
                    }
                  });

                // Now remove them.
                seriesToRemove.forEach(function _remove(series) {
                  series.remove(false);
                });

                // Find the marker that matches the same category
                // as the unselected point
                var point = _.find(
                  self.timeGraph.series[1].data,
                  function findMatchingX(point) {
                    return (point.x === evt.point.x);
                  });

                // We should always have a matching point
                if (point) {
                  point.remove(false);
                }
              } else if (measurementScan) {
                // Get the next series color, resetting if necessary
                var scanColorIdx = self.graphModel.get('scanColorIdx');
                if (scanColorIdx === SCAN_COLORS.length) {
                  scanColorIdx = 0;
                }
                var color = SCAN_COLORS[scanColorIdx];

                // Get the next symbol for the color, resetting if necessary
                var symbolIdxs = self.graphModel.get('symbolIdxs');
                if (symbolIdxs[scanColorIdx] === SYMBOLS.length) {
                  symbolIdxs[scanColorIdx] = 0;
                }

                var symbol = SYMBOLS[symbolIdxs[scanColorIdx]++];

                // Update the indexes
                self.graphModel.set('symbolIdxs', symbolIdxs);

                // Now move onto the next color for the next time
                scanColorIdx++;
                self.graphModel.set('scanColorIdx', scanColorIdx);

                // Add a marker that shows the symbol that can be used
                // in the scan graph
                var columnHeightPx = evt.point.shapeArgs.height;
                // For each y value, we get some number of pixels
                var yUnitPx = columnHeightPx / evt.point.y;
                // we want to move it up 20px to get it within the column
                var adjustment = 15 / yUnitPx;

                self.timeGraph.series[1].addPoint({
                  x: evt.point.x,
                  y: evt.point.y + adjustment,
                  color: '#000',
                  name: id,
                  marker: {
                    symbol: symbol,
                    fillColor: color,
                    radius: 10,
                  },
                }, false);

                // Create the scan series for the clicked measurement
                var measurements = measurementScan.get('measurements');
                var instanceScanSeriesComponents =
                  self._createScanSeries(name, symbol);

                // Get the series
                var series = instanceScanSeriesComponents.series;

                // Lock the point so it doesn't go away until
                // the user wants it to
                options.className = 'locked';
                evt.point.update({
                  options: options,
                  color: color,
                }, false);

                // Sync the color and add the series to the chart
                series.forEach(function _addSeries(series) {
                  series.color = color;
                  series.showInLegend = false;
                  self.scanChart.addSeries(series, false);
                });
                // Add the data to the series
                self._addDataToSeries(name, measurements);
              }
              // We immediately redraw because the user would expect that
              self.scanChart.redraw();
              self.timeGraph.redraw();
            },
          },
        },
        {
          type: 'scatter',
          name: 'Chart Render Symbols',
          data: [],
          events: {
            'click': function removeScanFromGraphs(evt) {
              var point = evt.point;
              var name = point.options.name;
              // Find the series to remove that were associated with
              // this point in time.
              var seriesToRemove = [];
              self.scanChart.series.forEach(
                function _checkToRemove(series) {
                  if (series.name === name) {
                    seriesToRemove.push(series);
                  }
                });

              // Now remove them.
              seriesToRemove.forEach(function _remove(series) {
                series.remove(false);
              });

              // See if there is still a column point on the
              // graph and if some flip it back to unlocked
              var columnPoint = self.timeGraph.get(name);
              if (columnPoint) {
                // Remove the flag that it is locked
                columnPoint.options.className = '';
                columnPoint.update({
                  options: columnPoint.options,
                  color: null,
                }, false);
              }

              // Remove and redraw immediately
              evt.point.remove();
			  //redraw entire chart on removal
              self.scanChart.redraw();
            },
          },
          dataLabels: {
            enabled: false,
          },
          tooltip: {
            formatter: function() {
              return false;
            },
          },
          enableMouseTracking: true,
          showInLegend: false,
        },
        ],

        yAxis: [{
          title: {
            text: '',
          },
          height: '60%',
          top: '40%',
          visible: false,
        }, {
          id: SINGLE_MASS_SERIES_ID,
          title: {
            text: '',
          },
          height: '50%',
          top: '40%',
          visible: false,
        }],

        xAxis: {
          title: {
            text: null,
          },
          labels: {
            align: 'center',
            /**
             * Computes the scan end time, then converts it to a relative
             * time by seconds as a minimum and days as a maximum
             * @return {string} The text to display
             */
            formatter: function _toRelativeTime() {
              return formatTimeValue(this.value);
            },
          },
        },

        plotOptions: {
          series: {
            pointWidth: 54,
            pointPadding: 10,
            dataLabels: {
              enabled: true,
              inside: true,
            },
            states: {
              inactive: {
                opacity: 1,
              },
              select: {
                enabled: false,
              },
            },
          },
        },
      });

      // Add a resize handler to compute how many columns
      // are possible to display given the real estate
      this.graphModel.set(
        'resizeHandler',
        function resizeHandler() {
          this.graphModel.set('needsToRecomputeColumns', true);
        }.bind(this));
      $(window).on('resize', this.graphModel.get('resizeHandler'));

      // Add any already configured display elements
      this.ScanInfoElements.each(function(scanInfoElement) {
        self.addAdditionalDisplayElementoTimeGraph(scanInfoElement);
      });

      // Try to render whatever is currently available.
      this.graphModel.set('measurementScanAvailable', true);

      // Now update the graph
      this.updateMonitorGraph();
    },
    /**
     * Updates the extremes of the graph.
     */
    updateExtremes: function updateExtremes() {
      // Now grab that info and set the min and maxes so the
      // navigator is sized appropriately
      var lastScanIdx = this.timeGraph.series[0].data.length - 1;
      var latestScan = this.timeGraph.series[0].data[lastScanIdx];
      if (latestScan) {
        // We determine how many scans are displayable
        var startScan = this.timeGraph.series[0].
          data[lastScanIdx - this.graphModel.get('numOfColumns')];
        var startScanTime = null;
        if (startScan) {
          startScanTime = startScan.x;
        }
        // Update the mins and maxes which will
        // be reflected in the navigator
        this.timeGraph.xAxis[0].setExtremes(startScanTime, latestScan.x);
      }
    },
    /**
     * Redraws the scan chart
     */
    redrawScanChart: _.debounce(function redrawScanChart() {
      if (this.graphModel.get('scanDataAvailable')) {
        if (this.scanChart &&
          this.scanChart.xAxis[0].plotLinesAndBands.length) {
          // We don't want to waste time updating the line
          // if the rate is too fast.
          if (this.ScanSetup.get('scanTimeTotal') <
            CURRENT_SCAN_MIN_TIME) {
            if (this.scanChart.xAxis[0].plotLinesAndBands[0].svgElem.visibility
              === 'inherit') {
              this.scanChart.xAxis[0].plotLinesAndBands[0].svgElem.hide();
            }
          } else {
            // Show the line if it was hidden before.
            if (this.scanChart.xAxis[0].plotLinesAndBands[0].svgElem.visibility
              !== 'inherit') {
              this.scanChart.xAxis[0].plotLinesAndBands[0].svgElem.show();
            }

            // Update the location of the plot line
            var lastSeriesUpdated = this.graphModel.get('lastSeriesUpdated');
            if (lastSeriesUpdated && lastSeriesUpdated.options.data &&
              lastSeriesUpdated.options.data.length) {
              // From: https://jsfiddle.net/d_paul/vyh2j4zr/1/
              var plotLine =
                this.scanChart.xAxis[0].plotLinesAndBands[0];
              var lastPoint =
                lastSeriesUpdated.options.
                  data[this.graphModel.get('lastPointIndex')];
              var x;
              if (lastPoint) {
                x = lastPoint[0];
              } else {
                x = lastSeriesUpdated.options.data[0];
              }
              var d = plotLine.svgElem.d.split(' ');
              var newX = this.scanChart.xAxis[0].toPixels(x) - d[1];
              if (x < plotLine.options.value) {
                // We want it to bounce back to the beginning to give it more
                // of a flow
                var minX =
                  this.scanChart.xAxis[0].toPixels(
                    lastSeriesUpdated.options.data[0].x) -
                  d[1];
                plotLine.svgElem.animate({
                  translateX: minX,
                }, {
                  duration: 166,
                }, function() {
                  plotLine.svgElem.animate({
                    translateX: newX,
                  }, {
                    duration: 166,
                  });
                });
                // });
              } else {
                plotLine.svgElem.animate({
                  translateX: newX,
                }, {
                  duration: 332,
                });
              }
              plotLine.options.value = x;
            }
          }
        }
        this.scanChart.redraw();
        this.graphModel.set({
          'scanDataAvailable': false,
        }, {
          silent: true,
        });
      }
    }, REDRAW_RATE),
    /**
     * Creates a new series with the name and symbol, if passed in.
     * @param {string} name The name for the new scan series.
     * @param {string} [symbol] The symbol to use on the series.
     * @return {object} The object with the series, and X axis min and max.
     */
    _createScanSeries: function _createScanSeries(name, symbol) {
      var series = [];
      var xAxisMin = 500;
      var xAxisMax = 0;
      // Add a series for each channel that is scanning data
      this.ScanSetupChannels.forEach(function _processChannelData(channel) {
        if (channel.get('enabled') !== 'True') {
          return;
        }

        switch (channel.get('channelMode')) {
          case window.app.ScanSetupChannel.prototype.ChannelType.SWEEP:
            xAxisMin = Math.min(xAxisMin, channel.get('startMass'));
            xAxisMax = Math.max(
              xAxisMax,
              channel.get('stopMass') + (1 / channel.get('ppamu')));
            series.push({
              id: CHANNEL_SERIES_PREFIX + channel.id,
              name: name,
              data: [],
              pointStart: channel.get('startMass'),
              pointInterval: 1 / channel.get('ppamu'),
              type: channel.get('ppamu') === 1 ? 'column' : 'line',
              yAxis: 0,
              xAxis: 0,
              zIndex: symbol ? 1 : 1000,
            });
            if (symbol) {
              series[series.length - 1].marker = {
                symbol: symbol,
                lineWidth: 0,
                enabled: true,
              };
              series[series.length - 1].states = {
                normal: {
                  opacity: 0.75,
                },
              };
            } else {
              series[series.length - 1].marker = {
                enabled: false,
              };
            }
            break;
          case window.app.ScanSetupChannel.prototype.ChannelType.SINGLE:
            xAxisMin = Math.min(xAxisMin, channel.get('startMass') - 1);
            xAxisMax = Math.max(
              xAxisMax,
              channel.get('startMass') + 1);
            series.push({
              id: CHANNEL_SERIES_PREFIX + channel.id,
              name: name,
              data: [],
              pointStart: channel.get('startMass'),
              pointInterval: 1,
              type: 'column',
              yAxis: 0,
              xAxis: 0,
              zIndex: 1000,
            });
            break;
          default:
            break;
        }
      });

      return {
        series: series,
        xAxisMin: xAxisMin,
        xAxisMax: xAxisMax,
      };
    },
    /**
     * Updates the scan chart with the latest data
     */
    updateMonitorGraph: _.debounce(function updateMonitorGraph() {
      // Can't update something we don't have
      if (this.scanChart) {
        // We need at least one channel of sweep data
        var foundOne = false;
        this.ScanSetupChannels.forEach(function _findSweepChannel(channel) {
          if (channel.get('enabled') === 'True' &&
            (channel.get('channelMode') ===
              window.app.ScanSetupChannel.prototype.ChannelType.SWEEP ||
              channel.get('channelMode') ===
              window.app.ScanSetupChannel.prototype.ChannelType.SINGLE)) {
            foundOne = true;
          }
        });
        if (foundOne) {
          // Create the series for the current scan
          var currentScanSeriesComponents =
            this._createScanSeries(CURRENT_SCAN_NAME);
          var currentScanSeries = currentScanSeriesComponents.series;

          // Find all the series for the current scan
          var seriesToRemove = [];
          for (var i = 0; i < this.scanChart.series.length; i++) {
            if (this.scanChart.series[i].name === CURRENT_SCAN_NAME) {
              seriesToRemove.push(this.scanChart.series[i]);
            }
          }

          // Now remove them all.
          seriesToRemove.forEach(function _removeSeries(series) {
            series.remove(false);
          });

          var plotLines = [];
          // Now add in the new series
          for (var sIdx = 0; sIdx < currentScanSeries.length; sIdx++) {
            // If there is a line series, we want a bouncey line.
            if (currentScanSeries[sIdx].type === 'line' && !plotLines.length) {
              plotLines.push({
                value: currentScanSeriesComponents.xAxisMin,
                color: '#AAA',
                width: 2,
                dashStyle: 'dash',
              });
            }
            currentScanSeries[sIdx].color = Highcharts.theme.colors[0];
            currentScanSeries[sIdx].showInLegend = false;
            this.scanChart.addSeries(currentScanSeries[sIdx], false);
          }

          // Update the min max so the x-axis is fixed
          this.scanChart.xAxis[0].update({
            min: currentScanSeriesComponents.xAxisMin,
            max: currentScanSeriesComponents.xAxisMax,
            plotLines: plotLines,
          }, false);

          // Reflow the chart for spacing. Only needed on initial load
          this.scanChart.reflow();

          // Update series in case out of order
          // This will redraw the chart
          if (this.currentScan) {
            this.updateScanSeries(this.currentScan.get('measurements'));
          }
        }
      }
    }, 100),
    /**
     * Updates the scan series from the measurements passed in.
     * @param {window.app.MeasurementScan[]} measurements The
     * measurements containing the data to add to update the scan series.
     */
    updateScanSeries: function updateScanSeries(measurements) {
      // Can't update something we don't have
      if (this.scanChart) {
        // Update the data on the right series based on which channel
        // the data came in on.
        var i;
        if (!measurements || !measurements.length) {
          for (i = 0; i < this.scanChart.series.length; i++) {
            if (this.scanChart.series[i].name === CURRENT_SCAN_NAME) {
              this.scanChart.series[i].setData([]);
            }
          }
        } else {
          // Add the data to the series
          this._addDataToSeries(CURRENT_SCAN_NAME, measurements);
          // Now redraw the graph
          this.graphModel.set('scanDataAvailable', true);
        }
      }
    },
    /**
     * Adds the data from the measurements to the series by name.
     * @param {string} name The name of the series to add data to.
     * @param {window.app.MeasurementScan[]} measurements The
     * measurements containing the data to add to the actual graph series.
     */
    _addDataToSeries: function _addDataToSeries(name, measurements) {
      // Observed error on destroy
      if (this.scanChart.series) {
        // Key the measurements for quicker look ups
        var measureById = {};
        var i;
        for (i = 0; i < measurements.length; i++) {
          measureById[CHANNEL_SERIES_PREFIX + measurements[i].channelId] =
            measurements[i];
        }

        // The smallest value above zero for log graphs
        var minAboveZero;
        // get totalPressure if unit is PPM
        var unitType = this.graphModel.get("unitType");
        this.Measurement.fetch().then(function() {
          var totalPressure = this.Measurement.get("totalPressure");
          if (isNaN(totalPressure) || totalPressure === 0) {
            totalPressure = 1;
          } 
          // Go through each series and try to update the data
          for (i = 0; i < this.scanChart.series.length; i++) {
            // Copy the existing data and then update it.
            var existingData = this.scanChart.series[i].options.data.slice();

            // Undo multiplier to simplify re-multiplying after adding new pts
            var multiplier = this.graphModel.get("unitMultiplier");
            _.map(existingData, function(data) {
              data[1] /= multiplier;
              if(unitType === "PPM") {
                data[1] *= totalPressure;
              }
            });

            // Now go through the data
            // Only update the series for the scan name.
            if (this.scanChart.series[i].name === name &&
              measureById[this.scanChart.series[i].userOptions.id]) {
              // Get the values needed to compute the x value.
              // Iterate over all the data an either update the existing point,
              // or add a point if more data has been received than already
              // exists
              var data =
                measureById[this.scanChart.series[i].userOptions.id].data;
              for (var j = 0; j < data.length; j++) {
                var datum = data[j];
                // We recompute the x because we don't know what has already
                // been added versus what has been rendered
                var x = (j * this.scanChart.series[i].options.pointInterval) +
                  this.scanChart.series[i].options.pointStart;
                existingData[j] = [x, datum];

                // Only set the flags if the series is part of the current scan
                if (this.scanChart.series[i].name === CURRENT_SCAN_NAME) {
                  this.graphModel.set({
                    'lastPointIndex': j,
                    'lastSeriesUpdated': this.scanChart.series[i],
                  });
                }
              }

              // Find the latest min above zero value from all the data
              minAboveZero = _.reduce(existingData, function(currentMin, point) {
                var datum = point[1];
                if (datum > 0) {
                  return Math.min(currentMin, datum);
                } else {
                  return currentMin;
                }
              }, 1);

              // track scaling changes
              var needRescale = false;
              var gMin = this.graphModel.get("globalMin");
              var gMax = this.graphModel.get("globalMax");
  
              var unitMultiplier = this.graphModel.get("unitMultiplier");
              var convertedData = [];
              
              // perform unit calculations, check for rescales
              existingData.forEach(function(dataElements){
                dataElements[1] = dataElements[1] * unitMultiplier;
                if (unitType === "PPM") {
                  dataElements[1] = dataElements[1] / totalPressure;
                }
                convertedData.push(dataElements[1]);
              }.bind(this));
              
              // Find local min/max to compare to global
              var newMax = _.max(convertedData);
              var newMin = _.min(convertedData);

              // Check whether the scale needs to be increased OR decreased
              // to match local min/max
              if(newMax > gMax) {
                gMax = newMax;
                needRescale = true;
              }
              
              if(newMin < gMin) {
                gMin = newMin;
                needRescale = true;
              }
              
              // Perform rescale if necessary
              if (needRescale) {
                this.scanChart.yAxis[0].setExtremes(gMin, gMax);
                this.graphModel.set("globalMin", gMin);
                this.graphModel.set("globalMax", gMax);
              }

              this.scanChart.series[i].setData(existingData, false);
            }
          }

          // Update the min above zero used on the highcharts override to
          // allow for negative values
          this.scanChart.yAxis[0].userOptions.minAboveZero =
            minAboveZero !== 1 ? minAboveZero / 10 : null;
        }.bind(this));
      }
    },
    /**
     * Removes the view by cleanly destroying the graphs first.
     */
    remove: function _remove() {
      // Destroy the graphs
      if (this.scanChart) {
        this.scanChart.destroy();
        this.scanChart = null;
      }
      if (this.timeGraph) {
        this.timeGraph.destroy();
        this.timeGraph = null;
      }
      this.graphModel.off('change:unitType');
      // Turn off the resize handler on the window.
      $(window).off('resize', this.graphModel.get('resizeHandler'));
      // Call the base.
      window.app.BaseView.prototype.remove.apply(this, arguments);
    },
  });
})(jQuery);
