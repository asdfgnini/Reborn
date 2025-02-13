unitChange: function unitChange(){
    // divide by this to change units to original torr
    // 用这个值除以将单位转换为原始的 torr
    // 在不同的单位转换过程中，我们使用该值作为转换的基准，以便恢复为最初的torr单位
    var divisor = this.graphModel.get("unitMultiplier");

    // 从IonSource获取数据
    this.IonSource.fetch().then(function() {
      var Sensitivity = this.IonSource.get("ppSensitivityFactor");  // 获取敏感度因子
      var FragFactor = this.IonSource.get("ppLinConst1");  // 获取碎片因子
      var IonizationProbability = this.IonSource.get("ppLinConst2");  // 获取电离概率

      // 如果接收到的值不正确，将它们设置为已知常数
      // 以下检查确保了敏感度、碎片因子和电离概率不会因为无效值而影响计算
      if(isNaN(Sensitivity) || Sensitivity == 0){
        Sensitivity = 1;  // 如果敏感度无效或为零，设置为1
      }
      if(isNaN(FragFactor) || FragFactor == 0){
        FragFactor = 0.94;  // 如果碎片因子无效或为零，设置为默认值0.94
      }
      if(isNaN(IonizationProbability) || IonizationProbability == 0){
        IonizationProbability = 1;  // 如果电离概率无效或为零，设置为1
      }

      // 根据当前的单位类型来计算转换倍数
      switch(this.graphModel.get("unitType")) {
        case "Amps":
          this.graphModel.set("unitMultiplier", 1);  // 如果单位为Amps，则倍数为1
          break;
        case "Torr":
          // 如果单位为Torr，计算单位转换倍数
          this.graphModel.set("unitMultiplier", 1/(Sensitivity*FragFactor*IonizationProbability));
          break;
        case "mB":
          // 如果单位为mB，计算单位转换倍数
          this.graphModel.set("unitMultiplier", 1.333222/(Sensitivity*FragFactor*IonizationProbability));
          break;
        case "Pa":
          // 如果单位为Pa，计算单位转换倍数
          this.graphModel.set("unitMultiplier", 133.3222/(Sensitivity*FragFactor*IonizationProbability));
          break;
        case "PPM":
          // 如果单位为PPM，计算单位转换倍数
          this.graphModel.set("unitMultiplier", (1/(Sensitivity * FragFactor * IonizationProbability)) * 1000000);
          break;
        default:
          // 如果单位为其他类型，默认为“Amps”，并设置转换倍数为1
          this.graphModel.set("unitType", $.i18n('mmsp-amps'));  // 获取默认的单位名称
          this.graphModel.set("unitMultiplier", 1);
      }

      // 获取计算得到的新的单位转换倍数
      var newMultiplier = this.graphModel.get("unitMultiplier");

      // 如果存在时间图表，则对每个时间图表的系列数据进行单位转换
      if(this.timeGraph){
        this.timeGraph.series.forEach(function(series){
          let dataArr = [];
          // 对每个数据点应用新的单位转换倍数
          series.yData.forEach(function(data){
            dataArr.push(data*newMultiplier/divisor);  // 乘以新的倍数，并除以之前的倍数
          })
          series.setData(dataArr,true,true,true);  // 更新系列数据
        })
      }

      // 如果存在扫描图表，则对每个扫描图表的系列数据进行单位转换
      if(this.scanChart){
        this.scanChart.series.forEach(function(series){
          let dataArr = [];
          // 对每个数据点应用新的单位转换倍数
          series.yData.forEach(function(data){
            dataArr.push(data*newMultiplier/divisor);  // 乘以新的倍数，并除以之前的倍数
          })
          series.setData(dataArr,true,true,true);  // 更新系列数据
        })
        
        // 在单位改变时，重新绘制图表
        this.updateMonitorGraph();
        
        // 更新Y轴标签，以反映当前的单位类型
        this.scanChart.yAxis[0].setTitle({
          text: this.graphModel.get("unitType")  // 将Y轴标签更新为当前的单位类型
        });
      }

      // 清除当前图表的数据，准备更新数据
      this.clearGraphs();

      // 在单位改变时重置极值
      this.graphModel.set("globalMin", 0);  // 重置全局最小值
      this.graphModel.set("globalMax", 0);  // 重置全局最大值
    }.bind(this));  // 绑定this，确保在回调函数中访问正确的上下文      
  }
