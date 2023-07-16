#include "toolfactory.h"
#include "tools/sncroptool.h"
#include "tools/sncircletool.h"
#include "tools/snlinetool.h"
#include "tools/snpenciltool.h"
#include "tools/snrectangletool.h"
#include "tools/sntexttool.h"

SNToolFactory::SNToolFactory(QObject *parent)
    : QObject{parent}
{

}

SNAbstractTool* SNToolFactory::createTool(SNAbstractTool::Type type, const SNToolSettings& setting)
{
    SNAbstractTool *abstTool = nullptr;
    switch (type) {
    case SNAbstractTool::TYPE_PENCIL:
        abstTool = new SNPencilTool();
        break;

    case SNAbstractTool::TYPE_CIRCLE:
        abstTool = new SNCircleTool();
        break;

    case SNAbstractTool::TYPE_LINE:
        abstTool = new SNLineTool();
        break;

    case SNAbstractTool::TYPE_RECTANGLE:
        abstTool = new SNRectangleTool();
        break;

    case SNAbstractTool::TYPE_TEXT:
        abstTool = new SNTextTool();
        break;

    case SNAbstractTool::TYPE_CROP:
        abstTool = new SNCropTool();
        break;

    default:
        break;
    }

    if (abstTool)
        abstTool->applySetting(setting);

    return abstTool;
}
