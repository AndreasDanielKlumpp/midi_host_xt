//midi connector app for Linux based standalone midi host adapters (required: xterm, aconnect)

//Copyright (c) 2016-2023 A. Klumpp


// *  This program is free software; you can redistribute it and/or modify
// *  it under the terms of the GNU General Public License as published by
// *  the Free Software Foundation; either version 2 of the License, or
// *  (at your option) any later version.
// *
// *  This program is distributed in the hope that it will be useful,
// *  but WITHOUT ANY WARRANTY; without even the implied warranty of
// *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// *  GNU General Public License for more details.
// *
// *  You should have received a copy of the GNU General Public License
// *  along with this program; if not, write to the Free Software
// *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
// *



//install libasound2 and libasound2-dev

#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "arecordmidi.c"
#include <QDebug>
#include <QProcess>

#include <QDateTime>
#include <QFile>



#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>
#include <getopt.h>
#include <sys/poll.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>
#include <getopt.h>
#include <sys/poll.h>
#include <alsa/include/asoundlib.h>
//#include <alsa/include/aconfig.h>
#include <alsa/include/config.h.in>
#include <alsa/include/version.h>

//


#include <linux/soundcard.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int mrid;
int mrport;


QProcess process1;
QProcess process2;
QProcess process3;
QProcess process4;


static int queue;


QList<QString> nameAr;
QList<QString> clientAr;
QList<QString> portAr;
QList<QString> fullAr;

QList<QString> nameArIn;
QList<QString> clientArIn;
QList<QString> portArIn;
QList<QString> fullArIn;


int autoswitch1;
int autoswitch2;



//QString midiclient1r;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //

        //


#define perm_ok(pinfo,bits) ((snd_seq_port_info_get_capability(pinfo) & (bits)) == (bits))
snd_seq_t *seq;
snd_seq_open(&seq, "default", SND_SEQ_OPEN_DUPLEX, 0);
//

    mrid = snd_seq_client_id(seq);
    //qDebug() <<  mrid;

    mrport = snd_seq_set_client_name(seq, "AlsaConnector");
    //qDebug() <<  mrport;

    //
    snd_seq_client_info_t *cinfo;
    snd_seq_port_info_t *pinfo;
    //



        snd_seq_port_info_alloca(&pinfo);


        snd_seq_port_info_set_capability(pinfo,
                         SND_SEQ_PORT_CAP_WRITE |
                         SND_SEQ_PORT_CAP_SUBS_WRITE);
        snd_seq_port_info_set_type(pinfo,
                       SND_SEQ_PORT_TYPE_MIDI_GENERIC |
                       SND_SEQ_PORT_TYPE_APPLICATION);
        snd_seq_port_info_set_midi_channels(pinfo, 16);


        snd_seq_port_info_set_timestamping(pinfo, 1);
        snd_seq_port_info_set_timestamp_queue(pinfo, queue);




//



    snd_seq_client_info_alloca(&cinfo);
    snd_seq_port_info_alloca(&pinfo);
    snd_seq_client_info_set_client(cinfo, -1);
    while (snd_seq_query_next_client(seq, cinfo) >= 0)

    {
            snd_seq_port_info_set_client(pinfo, snd_seq_client_info_get_client(cinfo));
            snd_seq_port_info_set_port(pinfo, -1);

            int seldd = 0;
             int iseldd = 0;

            while (snd_seq_query_next_port(seq, pinfo) >= 0)


                //output


                if (    perm_ok
                                       (pinfo, SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ))
                                       //(pinfo, SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE))
                                           {
                                               QString pname = snd_seq_port_info_get_name(pinfo);
                                              //qDebug() << pname;
                                               nameAr.append(pname);
                                               int icnum = snd_seq_client_info_get_client(cinfo);
                                               QString scnum = QString::number(icnum);
                                              // qDebug() << scnum;
                                               clientAr.append(scnum);
                                               //
                                               const char* fullname = snd_seq_client_info_get_name(cinfo);
                                              // qDebug() << "fullname";
                                                   // qDebug() << fullname;
                                                    fullAr.append(fullname + pname);
                                               //
                                               int ipnum = snd_seq_port_info_get_port(pinfo);
                                               QString spnum = QString::number(ipnum);
                                             // qDebug() << spnum;
                                               portAr.append(spnum);
                                              seldd++;
                                           }






     }


    ui->comboBoxClients->addItems(fullAr);
   // ui->comboBox2->addItems(nameArIn);

    qDebug() << fullAr;
   //-





    QString midiclient1r;
    QFile file("midiclient1.txt");
    if(!file.open(QIODevice::ReadOnly))
    {

        ui->statusBar->showMessage("midiclient1.txt error");
    }

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine();
        midiclient1r = line;
        qDebug() << "midiclient1r file: ";
        qDebug() <<  midiclient1r;

    }

    file.close();

    int n=0;
    for (QString x : fullAr)
    {

     qDebug() << x;
     qDebug() <<  midiclient1r;
     if (midiclient1r == x)
     {
     //qDebug() <<  midiclient1r;
     qDebug() <<  n;
     autoswitch1 = 1;
     qDebug() <<  "autoswitch1";
     ui->comboBoxClients->setCurrentIndex(n);
     }
     n=n+1;

    }







    //ui->comboBoxClients->setCurrentIndex(ui->comboBoxClients->findData(midiclient1));
    //ui->comboBoxClients->setCurrentText(midiclient1);
    //ui->comboBoxClients->setCurrentIndex(ui->comboBoxClients->findText(midiclient1));
    //ui->comboBoxClients->setCurrentIndex(3);

   //-



    input();
}


void MainWindow::input()

{







//-

qDebug() << "input function";

#define perm_ok(pinfo,bits) ((snd_seq_port_info_get_capability(pinfo) & (bits)) == (bits))
snd_seq_t *seq;
snd_seq_open(&seq, "default", SND_SEQ_OPEN_DUPLEX, 0);
//

    mrid = snd_seq_client_id(seq);
    //qDebug() <<  mrid;

    mrport = snd_seq_set_client_name(seq, "AlsaConnector");
    //qDebug() <<  mrport;

    //
    snd_seq_client_info_t *cinfo;
    snd_seq_port_info_t *pinfo;
    //



        snd_seq_port_info_alloca(&pinfo);


        snd_seq_port_info_set_capability(pinfo,
                         SND_SEQ_PORT_CAP_WRITE |
                         SND_SEQ_PORT_CAP_SUBS_WRITE);
        snd_seq_port_info_set_type(pinfo,
                       SND_SEQ_PORT_TYPE_MIDI_GENERIC |
                       SND_SEQ_PORT_TYPE_APPLICATION);
        snd_seq_port_info_set_midi_channels(pinfo, 16);


        snd_seq_port_info_set_timestamping(pinfo, 1);
        snd_seq_port_info_set_timestamp_queue(pinfo, queue);


//


//


snd_seq_client_info_alloca(&cinfo);
snd_seq_port_info_alloca(&pinfo);
snd_seq_client_info_set_client(cinfo, -1);
while (snd_seq_query_next_client(seq, cinfo) >= 0)

{
        snd_seq_port_info_set_client(pinfo, snd_seq_client_info_get_client(cinfo));
        snd_seq_port_info_set_port(pinfo, -1);

        int seldd = 0;
         int iseldd = 0;

        while (snd_seq_query_next_port(seq, pinfo) >= 0)



            //input (write)
            if (    perm_ok
                    //(pinfo, SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ))
                    (pinfo, SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE))
                        {
                            QString ipname = snd_seq_port_info_get_name(pinfo);
                           qDebug() << ipname;
                            nameArIn.append(ipname);
                            int iicnum = snd_seq_client_info_get_client(cinfo);
                            QString iscnum = QString::number(iicnum);
                            qDebug() << iscnum;
                            clientArIn.append(iscnum);
                            //
                            const char* ifullname = snd_seq_client_info_get_name(cinfo);
                            //qDebug() << "ifullname";
                                 qDebug() << ifullname;
                                 fullArIn.append(ifullname + ipname);
                            //
                            int iipnum = snd_seq_port_info_get_port(pinfo);
                            QString ispnum = QString::number(iipnum);
                          // qDebug() << ispnum;
                            portArIn.append(ispnum);

                            qDebug() << "iseldd";
                            qDebug() << iseldd;


                            //iseldd++;

                        }

            //input end





 }


//ui->comboBoxClients->addItems(nameAr);
 ui->comboBox2->addItems(fullArIn);



 qDebug() << fullArIn;
//-





 QString midiclient2r;
 QFile file("midiclient2.txt");
 if(!file.open(QIODevice::ReadOnly))
 {

     ui->statusBar->showMessage("midiclient2.txt error");
 }

 QTextStream in(&file);

 while(!in.atEnd())
 {
     QString line = in.readLine();
     midiclient2r = line;
     qDebug() << "midiclient2r file: ";
     qDebug() <<  midiclient2r;

 }

 file.close();

 int n=0;
 for (QString x : fullArIn)
 {

  qDebug() << x;
  qDebug() <<  midiclient2r;
  if (midiclient2r == x)
  {
  //qDebug() <<  midiclient1r;
  qDebug() <<  n;
  autoswitch2 = 1;
  qDebug() <<  "autoswitch2";
  ui->comboBox2->setCurrentIndex(n);
  }
  n=n+1;

 }

 if ((autoswitch1==1) && (autoswitch2==1 ))
 {
 ui->pushButtonRec->animateClick();
 qDebug() << "autoconnected";
 }








 //ui->comboBoxClients->setCurrentIndex(ui->comboBoxClients->findData(midiclient1));
 //ui->comboBoxClients->setCurrentText(midiclient1);
 //ui->comboBoxClients->setCurrentIndex(ui->comboBoxClients->findText(midiclient1));
 //ui->comboBoxClients->setCurrentIndex(3);

//-



}



void MainWindow::slotrec()

{
//qDebug() << "Recording midi";


    //process1.start("gnome-terminal -x bash -c \"arecordmidi -l\"");
     //process3.start("gnome-terminal -x bash -c \"aconnect -o\"");
      //process2.start("gnome-terminal -x bash -c \"aconnect -i\"");


      int getmididev = ui->comboBoxClients->currentIndex();
      //qDebug() << getmididev;

      QString getclient =  clientAr[getmididev];
      int igetclient = getclient.toInt();
      qDebug() << igetclient;

      QString getport =  portAr[getmididev];
      int igetport = getport.toInt();
      qDebug() << igetport;


      //


      int getmididev2 = ui->comboBox2->currentIndex();
      //qDebug() << getmididev;

      QString getclient2 =  clientArIn[getmididev2];
      int igetclient2 = getclient2.toInt();
      qDebug() << igetclient2;

      QString getport2 =  portArIn[getmididev2];
      int igetport2 = getport2.toInt();
      qDebug() << igetport2;


      //QDateTime local(QDateTime::currentDateTime());

     //QString dt = local.toString("yyyyMMddhhmmss");

       //QString filename = "AlsaMidiRec" + dt + ".midi";

         //qDebug() << filename;





     //  QString acommand = "gnome-terminal -x bash -c \"arecordmidi -p " + getclient + ":" + getport + " " + filename;

     // aconnect [-options] sender receiver
      //    sender, receiver = client:port pair

    //  aconnect 28:0 32:0


      //QString acommand = "gnome-terminal -x bash -c \"aconnect " + getclient + ":" + getport + " " + getclient2 + ":" + getport2;
      //QString acommand = "xterm -hold -e \"aconnect " + getclient + ":" + getport + " " + getclient2 + ":" + getport2;
      QString acommand = "xterm -e \"aconnect " + getclient + ":" + getport + " " + getclient2 + ":" + getport2;





     qDebug() <<  acommand;

      process1.start(acommand);





 //qDebug() <<  mrid;




 ui->statusBar->showMessage("Connected");





}





void MainWindow::slotstop()

{
    int getmididev = ui->comboBoxClients->currentIndex();
    //qDebug() << getmididev;

    QString getclient =  clientAr[getmididev];
    int igetclient = getclient.toInt();
    qDebug() << igetclient;

    QString getport =  portAr[getmididev];
    int igetport = getport.toInt();
    qDebug() << igetport;


    //


    int getmididev2 = ui->comboBox2->currentIndex();
    //qDebug() << getmididev;

    QString getclient2 =  clientArIn[getmididev2];
    int igetclient2 = getclient2.toInt();
    qDebug() << igetclient2;

    QString getport2 =  portArIn[getmididev2];
    int igetport2 = getport2.toInt();
    qDebug() << igetport2;




    //

 //QString acommand = "gnome-terminal -x bash -c \"aconnect -d " + getclient + ":" + getport + " " + getclient2 + ":" + getport2;
 QString acommand = "xterm -e \"aconnect -d " + getclient + ":" + getport + " " + getclient2 + ":" + getport2;

qDebug() <<  acommand;

 process1.start(acommand);


 ui->statusBar->showMessage("");








      //aconnect -i


//process1.terminate(SIGINT);
//process.close();
}




void MainWindow::slotsave()

{
    int getmididev = ui->comboBoxClients->currentIndex();
    //qDebug() << getmididev;

    QString getclient =  clientAr[getmididev];
    int igetclient = getclient.toInt();
    qDebug() << igetclient;

    QString getport =  portAr[getmididev];
    int igetport = getport.toInt();
    qDebug() << igetport;

    //QString name1 =  nameAr[getmididev];
    QString name1 =  fullAr[getmididev];
    //int igetport = getport.toInt();
    qDebug() << name1;

    //QString midiclient1 = getclient + ":" + getport;
    QString midiclient1 = name1;

    QString filename="midiclient1.txt";
    QFile file(filename);
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << midiclient1 << endl;
    }


    //


    int getmididev2 = ui->comboBox2->currentIndex();
    qDebug() << "getmididev2:";
    qDebug() << getmididev2;

    QString getclient2 =  clientArIn[getmididev2];
    int igetclient2 = getclient2.toInt();
    qDebug() << "igetclient2:";
    qDebug() << igetclient2;

    qDebug() << clientArIn;

    qDebug() << portArIn;

    qDebug() << fullArIn;

    QString name2 =  fullArIn[getmididev2];
    //int igetport2 = getport2.toInt();
    //qDebug() << igetport2;
     qDebug() << "name2:";
    qDebug() << name2;

//


    //QString midiclient1 = getclient + ":" + getport;
    QString midiclient2 = name2;

    QString filename2="midiclient2.txt";
    QFile file2(filename2);
    if ( file2.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file2 );
        stream << midiclient2 << endl;
    }




 //QString acommand = "xterm -e \"aconnect -d " + getclient + ":" + getport + " " + getclient2 + ":" + getport2;

//qDebug() <<  acommand;

 //process1.start(acommand);


 ui->statusBar->showMessage("saved");








      //aconnect -i


//process1.terminate(SIGINT);
//process.close();
}



MainWindow::~MainWindow()
{
    delete ui;
}
