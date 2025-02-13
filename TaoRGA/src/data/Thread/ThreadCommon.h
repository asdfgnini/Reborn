#pragma once
#include <QMetaType>
#include <functional>

using WorkCallback		 = std::function<bool()>;
using WorkResultCallback = std::function<void(bool)>;
 // namespace TaoCommon
Q_DECLARE_METATYPE(WorkCallback);
Q_DECLARE_METATYPE(WorkResultCallback);
