#ifndef TAOSUGGESTBOX_H
#define TAOSUGGESTBOX_H

#include <QVariantMap>
#include <QWidget>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoSuggestBoxPrivate;
class TAO_EXPORT TaoSuggestBox : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(TaoSuggestBox)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(Qt::CaseSensitivity, CaseSensitivity)
public:
    explicit TaoSuggestBox(QWidget* parent = nullptr);
    ~TaoSuggestBox();
    void setPlaceholderText(const QString& placeholderText);

    void addSuggestion(const QString& suggestText, const QVariantMap& suggestData = {});
    void addSuggestion(TaoIconType::IconName icon, const QString& suggestText, const QVariantMap& suggestData = {});
    void removeSuggestion(const QString& suggestText);
    void removeSuggestion(int index);
Q_SIGNALS:
    Q_SIGNAL void suggestionClicked(QString suggestText, QVariantMap suggestData);
};

#endif // TAOSUGGESTBOX_H
