#include "plugins/init/init.hh"

namespace xui
{

const int InitPlugin::ID = 0;
static RegisterPlugin<xui::InitPlugin> X{ "InitPlugin"};

}
