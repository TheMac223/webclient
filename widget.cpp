#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    onf = false;
    changeEnabled();
    //socket_.readyRead();


    QObject::connect(&ssocket_,&QAbstractSocket::connected,this,&Widget::doConnected);
    QObject::connect(&ssocket_,&QAbstractSocket::disconnected,this,&Widget::doDisconnected);
    QObject::connect(&ssocket_,&QAbstractSocket::readyRead,this,&Widget::doReadyRead);

    QObject::connect(&tsocket_,&QAbstractSocket::connected,this,&Widget::doConnected);
    QObject::connect(&tsocket_,&QAbstractSocket::disconnected,this,&Widget::doDisconnected);
    QObject::connect(&tsocket_,&QAbstractSocket::readyRead,this,&Widget::doReadyRead);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::doConnected(){
    ui->pteMessage->insertPlainText("Connected\n");
    onf = true;
    changeEnabled();
}

void Widget::doDisconnected(){
    ui->pteMessage->insertPlainText("Disconnected\n");
    onf = false;
    changeEnabled();

}

void Widget::doReadyRead(){

    if(ui->cbSsl->isChecked())
        ui->pteMessage->insertPlainText(ssocket_.readAll()+"\n");
    else
        ui->pteMessage->insertPlainText(tsocket_.readAll()+"\n");

}

void Widget::on_pbConnect_clicked()
{
    if((ui->cbSsl->isChecked() && ui->cbTcp->isChecked()) || (!ui->cbSsl->isChecked() && !ui->cbTcp->isChecked()) ){
        return;
    }
    if(ui->cbSsl->isChecked())
    ssocket_.connectToHostEncrypted(ui->leHost->text(), ui->lePort->text().toUShort());
    else
    tsocket_.connectToHost(ui->leHost->text(), ui->lePort->text().toUShort());
}


void Widget::on_pbDisconnect_clicked()
{
    if(ui->cbSsl->isChecked())
        ssocket_.close();
    else
        tsocket_.close();
}


void Widget::on_pbSend_clicked()
{
    if(ui->cbSsl->isChecked())
        ssocket_.write(ui->pteSend->toPlainText().toUtf8());
    else
        tsocket_.write(ui->pteSend->toPlainText().toUtf8());

}

void Widget::changeEnabled(){
    ui->pbClear->setEnabled(!ui->pteMessage->toPlainText().isEmpty());
    ui->pbConnect->setEnabled(!onf);
    ui->pbDisconnect->setEnabled(onf);
    ui->pbSend->setEnabled(onf);
}

void Widget::on_pbClear_clicked()
{
    ui->pteMessage->clear();
    changeEnabled();
}



