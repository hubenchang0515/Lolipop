#include <QApplication>
#include <QCommandLineParser>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

#if QT_VERSION >=  QT_VERSION_CHECK(5, 14, 0)
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif

    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption singleton{{"s", "singleton"}, "Run in singleton mode."};
    parser.addOption(singleton);
    parser.addPositionalArgument("file", "File to play.", "[file]");
    parser.process(app);
    auto options = parser.positionalArguments();
    
    if (Channel::instance().detect() && !options.empty())
    {
        Channel::instance().write(options[0]);
        return 0;
    }
    else
    {
        MainWindow window{parser.isSet(singleton)};
        window.show();
        
        if (!options.empty())
        {
            window.open(options[0]);
        }
        return app.exec();
    }
    
}