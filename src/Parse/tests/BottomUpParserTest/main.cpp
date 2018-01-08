#include <Parse/ParseByRise.hpp>
#include <UserInterface/ConsoleUserInterface.hpp>

int main(void)
{
  ConsoleUserInterface consoleUserInterface;
  TranslationProcess translationProcess;
  VTrans3::BottomUpParser bottomUpParser(& consoleUserInterface, translationProcess);
//  bottomUpParser.
}
