//  Created By: Daniel Best

#include <QtCore/QCoreApplication>
#include <QStringList>
#include <iostream>

#include "Stucco.h"

using namespace std;

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  QStringList args = a.arguments();
  QString input;
  QString output;
  for(int i = 0; i < args.size(); i++)
  {
    QString lowarg = args.at(i).toLower();
    if(lowarg == "-i" || lowarg == "/i" || lowarg == "\\i"){
      // input file name
      if (args.size() > i+1) {
        input = args.at(i+1);
        i++;
      }
    }
    if(lowarg == "-o" || lowarg == "/o" || lowarg == "\\o"){
      // output file name
      if (args.size() > i+1) {
        output = args.at(i+1);
        i++;
      }
    }
  }

  if(input.isEmpty() || output.isEmpty()){
    cout << "Usage:" << endl;
    cout << "  -i:\tInput file name" << endl;
    cout << "  -o:\tOutput file name. Use - for stdout" << endl;
  }

  Stucco s;
  s.ParseFile(input, output);  
}
