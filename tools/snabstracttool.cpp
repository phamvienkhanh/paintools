#include "snabstracttool.h"

SNAbstractTool::SNAbstractTool(QObject *parent)
    : QObject{parent}
{

}

void SNAbstractTool::applySetting(const SNToolSettings &setting)
{
    m_setting = setting;
}
