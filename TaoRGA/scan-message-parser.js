

/* global _ */

window.app = window.app || {};  // 确保 window.app 对象存在，如果没有则初始化为空对象

(function initScanMessageParser() {
  'use strict';

// 消息常量定义
var WORD_SIZE = 4;  // 单词大小定义为4字节，这通常用于定义数据类型的大小
var BINARY_DATA_TYPE = 0x44;  // 二进制数据的类型标识，0x44 表示数据类型的一个常数值
var HEADER_MARKER = 0x0A0B0C0D;  // 消息头的标识符，用于标识消息的开始位置
var HEADER_MARKER_SIZE = 4;  // 标识符的大小，通常为4字节
var HEADER_SIZE_SIZE = 1;  // 消息头部分的大小字段的字节数，这里为1字节
var HEADER_DATA_HEADER_SIZE = 1;  // 数据头部分的大小字段，通常是1字节，用来描述数据块结构
var HEADER_DATA_SIZE_SIZE = 2;  // 数据大小字段的字节数，通常为2字节，用来描述数据的总大小
var HEADER_DATA_TYPE_SIZE = 2;  // 数据类型字段的大小，通常为2字节，用来标识数据类型
var DATA_HEADER_POINT_TOTAL_SIZE = 4;  // 数据头中的总数据点数量字段的大小，这里定义为4字节
var DATA_HEADER_SCAN_SIZE_SIZE = 4;  // 扫描大小字段的字节数，表示每个扫描的大小
var DATA_HEADER_COUNT_SIZE = 4;  // 数据点计数字段的字节数，用于存储数据点的数量
var DATA_POINT_SIZE = 4;  // 每个数据点的大小，通常为4字节，表示每个数据点的存储大小

/**
 * BinaryConverter 类是一个工具类，用于将二进制数据转换为不同的类型
 * 这个类提供了将二进制数据（如缓冲区）转为数值、字符串等类型的方法。
 * @type {app.BinaryConverter}
 */
var BinaryConverter = window.app.BinaryConverter;

/**
 * ScanMessageParser 类用于解析扫描消息，并保持解析的状态，以便能够逐步处理部分或连续的消息。
 * 该类的实例会根据提供的 ScanSetupChannels 设置进行数据解析。它可以处理数据流中的分段消息， 
 * 且支持按需、序列化解析，以便能够在接收到完整数据之前对部分数据进行处理。
 * 
 * @class window.app.ScanMessageParser
 * @param {window.app.ScanSetupChannels} ScanSetupChannels - 用于提供通道配置的对象，它告知该如何解析输入的数据流。
 * 该参数通常包含用于解析数据的规则或设置。
 */
function ScanMessageParser(ScanSetupChannels) {
  /**
   * 用于第一次解析的标志，目的是在遇到第一个扫描开始点之前，
   * 清除所有数据点，确保我们只处理实际的扫描数据。
   * @type {boolean}
   */
  var isFirst = true;

  /**
   * 数据缓冲区中的下一个点的索引，表示在服务器上的数据位置。
   * 该索引指示我们当前正在解析哪个数据点。
   * @type {integer}
   */
  var nextPointIndex = 0;

  /**
   * 消息解析的缓冲区。这个缓冲区存储未完全解析的消息，以便可以顺序地传入多个消息并正确解析。
   * 在解析过程中，可能会有部分消息被缓存，直到可以完整解析为止。
   */
  var buffer = [];

  /**
   * Scan Setup Channels，提供有关如何解析数据的配置信息。
   * 这可以包括数据格式、字段长度、消息头位置等设置，帮助解析器按照正确的方式解析数据。
   */
  this.ScanSetupChannels = ScanSetupChannels;

  /**
   * 解析二进制数据并将其转换为扫描数据的消息。
   * 该方法会根据 ScanSetupChannels 中定义的规则进行解析，确保数据按照扫描格式被正确处理。
   * @param {Uint8Array} byteArray - 需要解析的二进制数据数组。
   * 该参数是原始的二进制数据流（例如通过网络接收的扫描数据）。
   * @return {null|window.app.MeasurementScan[]} - 解析后的扫描数据数组，
   * 如果数据不完整或未解析，则返回 null。
   * 每个 MeasurementScan 代表一个完整的扫描数据对象。
   * 
   * 该方法的核心作用是将输入的二进制数据转换为实际的扫描数据结构，这些扫描数据可以用于后续分析。
   */
  this.parse = function parse(byteArray) {
    // 如果接收到的二进制数据为空，直接返回空对象
    if (!byteArray.length) {
      return {};
    }
  
    // 解析出的扫描数据数组
    var parsedScans = [];
    // 如果有剩余数据，表示当前扫描的数据尚未完全解析
    var partialScan;
    var offset = 0;
    // 默认假设接收到的数据是小端字节序
    var littleEndian = true;
    
    // 读取消息头部的标记值
    var header = BinaryConverter.readInt(
      byteArray,
      littleEndian,
      HEADER_MARKER_SIZE,
      offset
    );
  
    // 模拟器使用大端字节序，而实际产品使用小端字节序
    if (header !== HEADER_MARKER) {
      littleEndian = false;
      header = BinaryConverter.readInt(
        byteArray,
        littleEndian,
        HEADER_MARKER_SIZE,
        offset
      );
    }
  
    // 确保消息的开头标记正确
    if (header !== HEADER_MARKER) {
      throw new Error('Invalid message response:' + header);
    }
    
    // 消费了4个字节，更新偏移量
    offset += HEADER_MARKER_SIZE;
  
    // 读取头部大小（头部中包含的单词数量）
    var headerSize = BinaryConverter.readInt(
      byteArray,
      littleEndian,
      HEADER_SIZE_SIZE,
      offset
    );
    offset += HEADER_SIZE_SIZE;
  
    // 读取数据头部的大小（以字为单位）
    var dataHeaderSize = BinaryConverter.readInt( // eslint-disable-line
      byteArray,
      littleEndian,
      HEADER_DATA_HEADER_SIZE,
      offset
    );
    offset += HEADER_DATA_HEADER_SIZE;
  
    // 读取数据的大小（以字为单位）
    var dataSize = BinaryConverter.readInt( // eslint-disable-line
      byteArray,
      littleEndian,
      HEADER_DATA_SIZE_SIZE,
      offset
    );
    offset += HEADER_DATA_SIZE_SIZE;
  
    // 读取数据类型（以字为单位）
    var dataType = BinaryConverter.readInt(
      byteArray,
      littleEndian,
      HEADER_DATA_TYPE_SIZE,
      offset
    );
    offset += HEADER_DATA_TYPE_SIZE;
  
    // 如果数据类型不是预期的二进制数据类型，则抛出错误
    if (dataType !== BINARY_DATA_TYPE) {
      throw new Error('Expected binary data type. Got:' + dataType);
    }
  
    // 接下来的两个字节是预留字段，跳过
    // 接下来的四个字节是状态字段，当前未使用
  
    // 计算数据体开始的偏移位置
    var dataOffset = headerSize * WORD_SIZE;
  
    // 现在解析数据头部
    // 获取自扫描开始以来的总数据点数
    var pointsSinceStartScan = BinaryConverter.readInt(
      byteArray,
      littleEndian,
      DATA_HEADER_POINT_TOTAL_SIZE,
      dataOffset
    );
    dataOffset += DATA_HEADER_POINT_TOTAL_SIZE;
  
    // 获取扫描的大小（每个扫描包含的数据点数）
    var scanSize = BinaryConverter.readInt(
      byteArray,
      littleEndian,
      DATA_HEADER_SCAN_SIZE_SIZE,
      dataOffset
    );
    dataOffset += DATA_HEADER_SCAN_SIZE_SIZE;
  
    // 获取数据中的点数
    var dataPointCount = BinaryConverter.readInt(
      byteArray,
      littleEndian,
      DATA_HEADER_COUNT_SIZE,
      dataOffset
    );
    dataOffset += DATA_HEADER_COUNT_SIZE;
  
    // 计算当前扫描的编号
    // 扫描编号从1开始，因此我们计算当前点数并根据扫描大小计算扫描编号
    var scanNum = Math.floor(pointsSinceStartScan / scanSize) + 1;
  
// 遍历数据点，解析数据直到没有剩余点数或生成了一个完整的扫描
var idx = 0;  // 用于指示当前解析的数据点位置
var ranOutOfPoints = false;  // 标记是否已经没有更多的数据点可解析
// false 表示“还有数据点可以继续解析”。
// true 表示“已经没有数据点可以解析了”。
var pointCount = 0;  // 用于统计已解析的数据点数

// 确保至少有一个通道是启用的，否则返回null
// 如果在没有启用任何通道的情况下开始轮询数据，将会进入死循环
var oneEnabled = false;  // 用于标记是否存在启用的通道
for (var sscIdx = 0; sscIdx < ScanSetupChannels.length; sscIdx++) {
  // 遍历所有通道，检查是否有一个通道处于启用状态
  if (ScanSetupChannels.at(sscIdx).get('enabled') === 'True') {
    oneEnabled = true;  // 如果找到了启用的通道，标记为启用
    break;  // 一旦找到启用的通道，跳出循环
  }
}

// 如果没有找到任何启用的通道，返回null
if (!oneEnabled) {
  return null;  // 如果没有启用的通道，返回null，避免继续进行数据解析
}

// 处理第一次解析时的数据
// 如果是第一次解析，并且自扫描开始已经有数据点
if (isFirst && pointsSinceStartScan) {
  // 需要对齐到最新的扫描数据，找到正确的位置
  // 这里的对齐是指确保数据解析的起始位置正确，避免解析已经处理过的点
  // pointsSinceStartScan 表示自扫描开始以来的数据点数，scanSize 表示每次扫描的点数
  // 计算需要丢弃的点数，以对齐到当前扫描的起始位置
  var pointsToPurge = scanSize - (pointsSinceStartScan % scanSize);
  
  // 根据需要丢弃的点数调整数据偏移量，确保不处理不该处理的数据
  dataOffset += pointsToPurge * DATA_POINT_SIZE;  // 根据丢弃点数和每个数据点的大小调整偏移量
  pointCount += pointsToPurge;  // 更新已处理的数据点数
  scanNum += 1;  // 增加扫描次数，表示开始了一个新的扫描周期
}

    
    // 将 isFirst 标志设置为 false，表示已经处理过第一次数据
    isFirst = false;
  }
  // 当扫描设置通道和数据点仍然存在时，我们继续处理
  while (!ranOutOfPoints && ScanSetupChannels.length) 
  {
    // 遍历所有的扫描通道，直到没有更多的点可以处理
    for (var channelIdx = 0;
      channelIdx < ScanSetupChannels.length &&
      !ranOutOfPoints;
      channelIdx++) {
      var channel = ScanSetupChannels.at(channelIdx);

      // 只处理已启用的通道
      if (channel.get('enabled') !== 'True') {
        continue; // 如果当前通道未启用，跳过该通道
      }

      switch (channel.get('channelMode')) {
        // 特别处理扫频通道
        case window.app.ScanSetupChannel.prototype.ChannelType.SWEEP:
          // 计算该通道的数据点总数，包含开始质量和结束质量之间的扫描点数
          var totalChannelPoints =
            ((channel.get('stopMass') - channel.get('startMass')) *
              channel.get('ppamu')) + 1;
          
          // 遍历所有的扫描点
          for (var i = 0; i < totalChannelPoints; i++) {
            if (idx >= buffer.length) 
            {
              // 确保我们有足够的数据来填充缓冲区
              //dataOffset + DATA_POINT_SIZE 表示当前要解析的下一个数据点的起始位置。
              // 如果下一个数据点的起始位置（dataOffset + DATA_POINT_SIZE）超出了数据字节数组的总长度（byteArray.length），
              // 则说明剩下的数据不足以构成完整的数据点。
              if (dataOffset + DATA_POINT_SIZE > byteArray.length) 
              {
                ranOutOfPoints = true; // 如果数据不足，标记为结束
                break; // 跳出循环处理更多的数据点
              } 
              else 
              {
                  // 否则，从字节数组中读取一个浮点数据并将其添加到缓冲区
                  buffer.push(
                  BinaryConverter.readFloat32(
                    byteArray,
                    littleEndian,
                    dataOffset
                  )
                );
                dataOffset += DATA_POINT_SIZE; // 更新偏移量
                pointCount++; // 增加数据点数量
              }
            }
            idx++; // 增加当前处理的数据点索引
          }
          break;

        // 处理单通道、TPQUAD、总压力和基线通道
        case window.app.ScanSetupChannel.prototype.ChannelType.SINGLE:
        case window.app.ScanSetupChannel.prototype.ChannelType.TPQUAD:
        case window.app.ScanSetupChannel.prototype.ChannelType.TOTAL_PRESSURE:
        case window.app.ScanSetupChannel.prototype.ChannelType.BASELINE:
          // 特殊的通道，不同的处理方式
          if (idx >= buffer.length) 
            {
            // 如果缓冲区未满，确保有足够的数据来填充
            if (dataOffset + DATA_POINT_SIZE > byteArray.length) {
              ranOutOfPoints = true; // 如果数据不足，标记为结束
            } else {
              // 从字节数组中读取一个浮点数据并将其添加到缓冲区
              buffer.push(
                BinaryConverter.readFloat32(
                  byteArray,
                  littleEndian,
                  dataOffset
                )
              );
              dataOffset += DATA_POINT_SIZE; // 更新偏移量
              pointCount++; // 增加数据点数量
            }
          }
          idx++; // 增加当前处理的数据点索引
          break;

        // 默认情况下，重新解析数据点
        default:
          // 只有在缓冲区未满时才重新解析数据
          if (idx >= buffer.length) {
            if (dataOffset + DATA_POINT_SIZE > byteArray.length) {
              ranOutOfPoints = true; // 如果数据不足，标记为结束
            } else {
              // 根据通道的类型，解析不同格式的数据
              if (channel.get('channelType') === 'F32Value') {
                buffer.push(
                  BinaryConverter.readFloat32(
                    byteArray,
                    littleEndian,
                    dataOffset
                  )
                );
              } else {
                // 默认读取整型数据
                buffer.push(
                  BinaryConverter.readInt(
                    byteArray,
                    littleEndian,
                    DATA_POINT_SIZE,
                    dataOffset
                  )
                );
              }
              dataOffset += DATA_POINT_SIZE; // 更新偏移量
              pointCount++; // 增加数据点数量
            }
          }
          idx++; // 增加当前处理的数据点索引
          break;
      }
    }
  // 解析已获取的数据
  var scanAttributes = this.toAttributes(scanNum, scanSize, buffer);  // 调用 toAttributes 方法将原始数据（扫描编号、扫描大小和扫描缓冲区）转化为结构化的扫描属性

// 如果数据点不足
  if (ranOutOfPoints) 
    {
      // 检查头部的点数与实际点数是否匹配
      if (pointCount !== dataPointCount) {
        console.error(
          'Header point count does not match actual',  // 如果头部的点数与实际的点数不匹配，则输出错误信息
          pointCount, dataPointCount);
      }
      // 如果点数不完整，我们将当前的扫描属性保存为部分扫描数据
      partialScan = scanAttributes;
    } 
    else 
    {
      // 如果缓冲区的大小与扫描大小不匹配，输出错误信息
      if (buffer.length !== scanSize) {
        console.error(
          'Buffer does not match scan size',  // 如果扫描缓冲区的长度与预期的扫描大小不符，输出错误信息
          buffer.length, scanSize, pointCount, dataPointCount);
      }
      // 创建一个新的测量扫描对象（MeasurementScan），并将其添加到解析的扫描数组中
      var mScan = new window.app.MeasurementScan(scanAttributes);  // 通过传入扫描属性，创建测量扫描对象
      parsedScans.push(mScan);  // 将扫描对象添加到解析的扫描数组中
      scanNum++;  // 增加扫描编号
      buffer = [];  // 清空缓冲区，准备处理下一个扫描数据
    }
  }

  // 设置下一个点的索引
  nextPointIndex = pointsSinceStartScan + pointCount;  // 更新下一个点的索引，依据已经处理的点和当前点的数量

  // 返回解析的扫描数据和部分扫描数据，供后续使用
  return {
    scans: parsedScans,  // 包含所有已解析的完整扫描对象
    partialScan: partialScan,  // 如果有部分扫描数据，则返回该数据
  };

  /**
   * 获取下一个点的索引，基于已解析的扫描数据。
   * 这个方法帮助我们追踪下一个应该处理的点。
   * @return {integer} 下一个点的索引。
   */

  this.getNextPointIndex = function getNextPointIndex() {
    return nextPointIndex;  // 返回当前的下一个点的索引
  };

/**
 * 将扫描数据解析成一个对象，包含扫描的详细属性，
 * 该方法根据不同的扫描通道类型处理数据。
 * @param {number} scanNum - 扫描的编号，用于标识当前扫描。
 * @param {number} scanSize - 扫描的大小，标识扫描数据的长度。
 * @param {number[]} scanValues - 扫描值数组，包含所有的原始数据点。
 * @return {object} 返回一个包含扫描属性和数据的对象。
 */
this.toAttributes = function toAttrbutes(scanNum, scanSize, scanValues) {
  var idx = 0;  // 初始化索引，用于在扫描值数组中定位
  var measurements = [];  // 存储解析后的测量数据
  var parsedData = {
    id: scanNum,  // 扫描的唯一标识
    scannum: scanNum,  // 扫描编号
    scansize: scanSize,  // 扫描的大小
  };

  // 遍历所有扫描通道，解析每个通道的数据
  this.ScanSetupChannels.forEach(function _parseData(channel) {
    // 只处理已启用且有数据的通道
    if (channel.get('enabled') !== 'True' || idx >= scanValues.length) {
      return;  // 如果通道未启用或没有数据，跳过该通道
    }

    // 根据通道的模式解析不同的数据类型
    switch (channel.get('channelMode')) {
      // 扫频通道（Sweep），计算并解析所有数据点
      case window.app.ScanSetupChannel.prototype.ChannelType.SWEEP:
        var channelTotalPoints;
        if (channel.get('channelMode') ===
          window.app.ScanSetupChannel.prototype.ChannelType.SWEEP) {
          // 扫频通道：计算该通道总共有多少数据点
          channelTotalPoints =
            ((channel.get('stopMass') - channel.get('startMass')) *
              channel.get('ppamu')) + 1;  // 总数据点 = (停止质量 - 开始质量) * 每单位质量的点数 + 1
        } else {
          channelTotalPoints = channel.get('ppamu') + 1;  // 如果不是扫频通道，仅根据每单位质量的点数计算
        }
        // 保存当前通道的扫描数据
        measurements.push({
          channelId: channel.id,  // 当前通道的ID
          stopMass: channel.get('stopMass'),  // 当前通道的停止质量
          startMass: channel.get('startMass'),  // 当前通道的开始质量
          ppamu: channel.get('ppamu'),  // 每单位质量的点数
          data: scanValues ? 
            scanValues.slice(
              idx, 
              idx + channelTotalPoints
            ) : [],  // 读取该通道的数据点
        });
        idx += channelTotalPoints;  // 更新数据点索引
        break;

      // 单通道数据（Single），每次读取一个数据点
      case window.app.ScanSetupChannel.prototype.ChannelType.SINGLE:
        measurements.push({
          channelId: channel.id,  // 当前通道的ID
          stopMass: channel.get('stopMass'),  // 当前通道的停止质量
          startMass: channel.get('startMass'),  // 当前通道的开始质量
          ppamu: channel.get('ppamu'),  // 每单位质量的点数
          data: [scanValues[idx]],  // 只取一个数据点
        });
        idx++;  // 更新数据点索引
        break;

      // 对于其他类型的数据（如TPQUAD、总压力、基线等），认为它们是标量值
      default:
        parsedData[channel.get('channelMode')] = scanValues[idx];  // 将标量值存储到parsedData中
        idx++;  // 更新数据点索引
        break;
    }
  });

  // 额外的业务逻辑处理：如果EM开启了，我们将信号的数据进行翻转
  var systemStatus = parsedData.SystemStatus;  // 获取系统状态
  if (_.isNumber(systemStatus)) {  // 如果系统状态是数字类型
    var emOn = Boolean(systemStatus & (1 << 23));  // 检查EM开关是否开启（根据系统状态的第23位）
    if (emOn) {
      // 如果EM开关开启，翻转所有测量数据的符号
      _.each(measurements, function flipMeasurements(measurement) {
        for (var i = 0; i < measurement.data.length; i++) {
          measurement.data[i] *= -1;  // 翻转数据的符号
        }
      });
    }
  }

    // 设置最终的测量数据
    parsedData.measurements = measurements;  // 将解析后的测量数据加入到 parsedData 中
    return parsedData;  // 返回完整的扫描数据对象
  };
  }
window.app.ScanMessageParser = ScanMessageParser;
})();

