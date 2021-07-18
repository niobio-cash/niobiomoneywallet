// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2017 - 2019 Niobio Cash developers - Derived work from -Karbowanec-
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include <QApplication>
#include <QCommandLineParser>
#include <QLocale>
#include <QTranslator>
#include <QLockFile>
#include <QMessageBox>
#include <QSplashScreen>
#include <QStyleFactory>

#include "CommandLineParser.h"
#include "CurrencyAdapter.h"
#include "LoggerAdapter.h"
#include "NodeAdapter.h"
#include "Settings.h"
#include "SignalHandler.h"
#include "WalletAdapter.h"
#include "gui/MainWindow.h"
#include "Update.h"
#include <QTextCodec>
#include <QFontDatabase>
#include <Common/Util.h>
#include "PaymentServer.h"
#include "WalletNodes.h"

#define DEBUG 1
using namespace WalletGui;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "-Wclazy-qt-macros"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName(CurrencyAdapter::instance().getCurrencyName() + "wallet");
    app.setApplicationVersion(Settings::instance().getVersion());
    app.setQuitOnLastWindowClosed(false);

    QFontDatabase::addApplicationFont(":/fonts/sourcecodepro");
    QFontDatabase::addApplicationFont(":/fonts/quan");
    QFontDatabase::addApplicationFont(":/fonts/mplusm");

    /*QFontDatabase db;
    for(int i=0; i<db.families().size(); i++)
    {
        qDebug() << db.families().at(i);
    }*/

#ifndef Q_OS_MAC
    QApplication::setStyle(QStyleFactory::create("Fusion"));
#endif

    CommandLineParser cmdLineParser(nullptr);
    Settings::instance().setCommandLineParser(&cmdLineParser);
    bool cmdLineParseResult = cmdLineParser.process(app.arguments());
    Settings::instance().load();
    QTranslator translator;
    QTranslator translatorQt;

    QString lng = Settings::instance().getLanguage();

    if (!lng.isEmpty()) {
        translator.load(":/languages/" + lng + ".qm");
        translatorQt.load(":/languages/qt_" + lng + ".qm");

        /*if(lng == "uk") {
              QLocale::setDefault(QLocale("uk_UA"));
          } else if(lng == "ru") {
              QLocale::setDefault(QLocale("ru_RU"));
          } else if(lng == "pl") {
              QLocale::setDefault(QLocale("pl_PL"));
          } else if(lng == "be") {
              QLocale::setDefault(QLocale("be_BY"));
          } else if(lng == "de") {
              QLocale::setDefault(QLocale("de_DE"));
          } else */if (lng == "es") {
            QLocale::setDefault(QLocale("es_ES"));
        } else {
            QLocale::setDefault(QLocale::c());
        }

    } else {
        translator.load(":/languages/" + QLocale::system().name());
        translatorQt.load(":/languages/qt_" + QLocale::system().name());
        QLocale::setDefault(QLocale::system().name());
    }
    app.installTranslator(&translator);
    app.installTranslator(&translatorQt);

    setlocale(LC_ALL, "");

    QFile File(":/skin/default.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    qApp->setStyleSheet(StyleSheet);

    if (PaymentServer::ipcSendCommandLine())
        exit(0);

    PaymentServer* paymentServer = new PaymentServer(&app);

#ifdef Q_OS_WIN
    if(!cmdLineParseResult) {
    QMessageBox::critical(nullptr, QObject::tr("Error"), cmdLineParser.getErrorText());
    return app.exec();
  } else if (cmdLineParser.hasHelpOption()) {
    QMessageBox::information(nullptr, QObject::tr("Help"), cmdLineParser.getHelpText());
    return app.exec();
  }
#endif

    LoggerAdapter::instance().init();

    QString dataDirPath = Settings::instance().getDataDir().absolutePath();

    if (!Tools::directoryExists(dataDirPath.toStdString())) {
        if (!Tools::create_directories_if_necessary(dataDirPath.toStdString())) {
            throw std::runtime_error("Can't create directory: " + dataDirPath.toStdString());
        }
    }

    QLockFile lockFile(Settings::instance().getDataDir().absoluteFilePath(QApplication::applicationName() + ".lock"));
    if (!lockFile.tryLock()) {
        QMessageBox::warning(nullptr, QObject::tr("Fail"), QObject::tr(
                "%1 wallet already running or I cannot create lock file %2. Check your permissions.").arg(
                CurrencyAdapter::instance().getCurrencyDisplayName()).arg(
                Settings::instance().getDataDir().absoluteFilePath(QApplication::applicationName() + ".lock")));
        return 0;
    }
    auto wNodes = new WalletNodes;
    wNodes->GetWalletNodes();

    SignalHandler::instance().init();
    QObject::connect(&SignalHandler::instance(), &SignalHandler::quitSignal, &app, &QApplication::quit);

    QSplashScreen *splash = new QSplashScreen(QPixmap(":images/splash"), /*Qt::WindowStaysOnTopHint |*/
                                              Qt::X11BypassWindowManagerHint);
    if (!splash->isVisible()) {
        splash->show();
    }
    //splash->setFont(monospace);
    //splash->showMessage(QObject::tr("Loading blockchain..."), Qt::AlignLeft | Qt::AlignBottom, Qt::white);

    app.processEvents();
    qRegisterMetaType<CryptoNote::TransactionId>("CryptoNote::TransactionId");
    qRegisterMetaType<quintptr>("quintptr");
    NodeAdapter::instance().init();
    /*if (!NodeAdapter::instance().init()) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Node does not respond, choose another node and try again!");
        messageBox.setFixedSize(500,200);
    }*/
    splash->finish(&MainWindow::instance());
    Updater d;
    d.checkForUpdate();

    MainWindow::instance().show();

    WalletAdapter::instance().open("");

    QTimer::singleShot(10000, paymentServer, SLOT(uiReady()));
    QObject::connect(paymentServer, &PaymentServer::receivedURI, &MainWindow::instance(),
                     &MainWindow::handlePaymentRequest, Qt::QueuedConnection);

    QObject::connect(QApplication::instance(), &QApplication::aboutToQuit, []() {
        MainWindow::instance().quit();
        if (WalletAdapter::instance().isOpen()) {
            WalletAdapter::instance().close();
        }

        NodeAdapter::instance().deinit();
    });

    return app.exec();

}


#pragma clang diagnostic pop
