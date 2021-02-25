#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->server = nullptr;
    this->client = nullptr;
    this->host_is_empty = true;
    this->port_is_empty = true;
    this->connected = false;
}

MainWindow::~MainWindow()
{
    on_ButtonDisconnect_clicked();
    delete ui;
}

void MainWindow::on_ButtonSend_clicked()
{
    QString text = ui->TextEdit->toPlainText();
    client->write(text.toUtf8());
}

void MainWindow::on_ButtonRecieve_clicked()
{
    QByteArray array = client->readAll();
    ui->TextEdit->setText(QString(array));
}

void MainWindow::on_ButtonConnect_clicked()
{
    if(!(valid_host_port())) {
        return;
    } else if(this->client == nullptr) {
        this->client = new QTcpSocket(this);
        QString host = ui->LineEditHost->text();
        int port = ui->LineEditPort->text().toInt();
        this->client->connectToHost(host, port);

        ui->LabelStatus->setText("Connected to server");

        enable_connection_ui(false);
    }
}

void MainWindow::on_ButtonListen_clicked()
{
    if(!(valid_host_port())) {
        return;
    } else if(this->server == nullptr){
        this->server = new QTcpServer(this);
        int port = ui->LineEditPort->text().toInt();
        this->server->listen(QHostAddress::Any, port);
        ui->LabelStatus->setText("Listening...");
        connect(server, &QTcpServer::newConnection, this, &MainWindow::connect_to_client);
    }
}

void MainWindow::on_ButtonDisconnect_clicked()
{
    if(this->client != nullptr) {
        this->client->close();
        delete this->client;
        this->client = nullptr;
    }
    if(this->server != nullptr) {
        this->server->close();
        delete this->client;
        this->server = nullptr;
    }
    ui->LabelStatus->setText("Disconnected.");
    enable_connection_ui(true);
}

void MainWindow::on_LineEditHost_textEdited(const QString &text)
{
    if(text == "") {
        host_is_empty = true;
    } else {
        host_is_empty = false;
        this->host = ui->LineEditHost->text();
    }
}

void MainWindow::on_LineEditPort_textEdited(const QString &text)
{
    if(text == "") {
        port_is_empty = true;
    } else {
        port_is_empty = false;
        this->port = ui->LineEditPort->text();
    }
}

bool MainWindow::valid_host_port()
{
    QRegularExpression regex("^(\\d{2,5})$");
    QRegularExpressionMatch match = regex.match(this->port);
    if(!(match.hasMatch())) {
        QMessageBox msg;
        msg.setText("Error: Port not valid");
        msg.exec();
        return false;
    }

    regex.setPattern("^(([0-9]{1,3}\\.){3,3}[0-9]{1,3})$|^(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4})$");
    regex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    match = regex.match(this->host);
    if(!(match.hasMatch())) {
        QMessageBox msg;
        msg.setText("Error: Host not valid");
        msg.exec();
        return false;
    }
    return true;
}

void MainWindow::connect_to_client()
{
    this->client = server->nextPendingConnection();
    ui->LabelStatus->setText("Connected To client");
    enable_connection_ui(false);

}

void MainWindow::enable_connection_ui(bool enable)
{
    // Enable Components
    ui->ButtonConnect->setEnabled(enable);
    ui->ButtonListen->setEnabled(enable);
    ui->LineEditHost->setEnabled(enable);
    ui->LineEditPort->setEnabled(enable);

    // Disable Components
    ui->ButtonDisconnect->setDisabled(enable);
    ui->ButtonSend->setDisabled(enable);
    ui->ButtonRecieve->setDisabled(enable);

    // Set variables
    this->connected = !enable;
}
