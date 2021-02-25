#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMessageBox>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void enable_connection_ui(bool enable);
    ~MainWindow();

private slots:
    void on_ButtonConnect_clicked();
    void on_ButtonListen_clicked();
    void on_ButtonDisconnect_clicked();
    void on_ButtonSend_clicked();
    void on_ButtonRecieve_clicked();
    void on_LineEditHost_textEdited(const QString &text);
    void on_LineEditPort_textEdited(const QString &text);
    void connect_to_client();
    bool valid_host_port();

private:
    Ui::MainWindow *ui;
    QTcpSocket *client;
    QTcpServer *server;
    bool connected;
    bool host_is_empty;
    bool port_is_empty;
    QString host;
    QString port;
};
#endif // MAINWINDOW_H
