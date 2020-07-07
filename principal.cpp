#include "principal.h"
#include "ui_principal.h"
#include <QDebug>
#include <QFileDialog>

Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    ui->inNombre->setFocus();
    connect(ui->cmdCalcular, SIGNAL(released()),this,SLOT(calcular()));
    connect(ui->mnuCalcular,SIGNAL(triggered(bool)),
            this,SLOT(calcular()));
    connect(ui->mnuSalir,SIGNAL(triggered(bool)),
            this,SLOT(close()));
    connect(ui->mnuGuardar, SIGNAL(triggered(bool)),
            this,SLOT(guardar()));
    connect(ui->mnuAbrir, SIGNAL(triggered(bool)),
            this,SLOT(abrir()));
}

Principal::~Principal()
{
    delete ui;
}

void Principal::calcular()
{
    //obtener datos
    int horas = ui->inHoras->value();




    QString nombre = ui->inNombre->text();
    qDebug()<<horas;
    if(nombre==""||horas==0){
        ui->statusbar->showMessage("No se ha ingresado datos para calcular.",5000);
        return ;
    }else{
        ui->statusbar->clearMessage();
    }
    float const HORA_EXTRA=20.10;
    //calcular horas extra
    int extra=0;
    if(horas>40){
        extra = horas - 40;
        horas = 40;

    }
    //Turno
    QString turno="";

    //calculo del salario
    float salario = 0;
    if(ui->inMatutina->isChecked()){
        salario = horas * 5.15+ extra * HORA_EXTRA;
        turno="Matutina";
    }
    else if ( ui->inVespertina->isChecked()){
        salario=horas*8.50+extra*HORA_EXTRA;
        turno="Vespertina";
    }else{
        salario = horas * 12.65 + extra*HORA_EXTRA;
        turno="Nocturna";
    }

    //calcular el descuento
    float descuento = salario * (9.5/100);
    float salario_neto=salario-descuento;
    //


    //Imprimir resultado
    QString resultado = "\nObrero: "+nombre +"\n";
    resultado += "Horas: "+ QString::number(horas) +"\n";
    resultado += "Jornada: "+turno+"\n";
    resultado += "Salario: $"+QString::number(salario)+"\n";
    resultado += "Descuento (9.5%): $"+QString::number(descuento)+"\n";
    resultado += "Salario neto: $"+QString::number(salario_neto)+"\n";
    resultado += "----------------------------------\n";

    ui->outResultado->appendPlainText(resultado);
    limpiar();
}

void Principal::guardar()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Guardar datos",QDir::home().absolutePath(),"Archivo de Texto(*.txt)");
    QFile data(fileName);
    if(data.open(QFile::WriteOnly|QFile::Truncate)){
        QTextStream salida(&data);
        salida<<ui->outResultado->toPlainText();
        ui->statusbar->showMessage("Datos almacenados");
    }
    data.close();
}

void Principal::abrir()
{
    QFile file(QFileDialog::getOpenFileName(this,tr("Open file"),"",tr("Text Files(*.txt)")));

       if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           return;

       QTextStream in(&file);
       while (!in.atEnd()) {
           QString linea = in.readLine();
           ui->outResultado->appendPlainText(linea);
       }

       file.close();
}

void Principal::limpiar()
{
    ui->inNombre->setText("");
    ui->inHoras->setValue(0);
    ui->inMatutina->setChecked(true);
    ui->inNombre->setFocus();

}

