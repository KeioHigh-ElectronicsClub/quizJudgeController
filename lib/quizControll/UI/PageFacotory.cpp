#include "PageFactory.h"

PageFactory::PageFactory(TFT_eSPI* display, RecodeApplicationService* recodeApp,
                         Config* config, ButtonInput* button) {
  this->display = display;
  this->recodeApp = recodeApp;
  this->config = config;
  this->button = button;
}

std::unique_ptr<IPage> PageFactory::create(PageList page,
                                           IPageChange* changer) {
  switch (page) {
    case PageList::Main:
      return std::unique_ptr<IPage>(
          new MainPage(display, changer, button, recodeApp));
      break;
    case PageList::Menu:
      return std::unique_ptr<IPage>(new MenuPage(display, changer, button));
      break;
    case PageList::ConfigLimit:
      return std::unique_ptr<IPage>(
          new ConfigLimit(display, changer, button, recodeApp, config));
    default:
      return nullptr;
  }
}
