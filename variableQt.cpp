#include "variableQt.h"

std::set<std::string> variableQt::sumvars;

variableQt::variableQt(std::vector<Variable*>* mvars, QString predicat) : vars(mvars)
{
  
  resize(QApplication::desktop()->width()/2, QApplication::desktop()->height()/2);
  move(QApplication::desktop()->screen()->rect().center() - rect().center());
    mainlayout = new QVBoxLayout(this);
    lab_pred = new QLabel(predicat);
    mainlayout->addWidget(lab_pred);
    lab_var = new QLabel(QString::fromUtf8(mvars->at(0)->get_name().c_str()));
    mainlayout->addWidget(lab_var);
    secondlayout = new QHBoxLayout();
    layout = new QVBoxLayout();
    line_var = new QLineEdit();
    but_ok = new QPushButton("OK");
    layout->addWidget(line_var);
    layout->addWidget(but_ok);
    layout2 = new QVBoxLayout();
    list_var = new QListWidget();
    for(std::set<std::string>::iterator it = variableQt::sumvars.begin(); it != variableQt::sumvars.end(); ++it){
      list_var->addItem(QString::fromUtf8((*it).c_str()));
    }
    but_ok_list = new QPushButton("OK");
    layout2->addWidget(list_var);
    layout2->addWidget(but_ok_list);
    numVars = 0;
    secondlayout->addLayout(layout);
    secondlayout->addLayout(layout2);
    mainlayout->addLayout(secondlayout);
    QObject::connect(but_ok, SIGNAL(clicked()), this, SLOT(ok()));
    QObject::connect(but_ok_list, SIGNAL(clicked()), this, SLOT(ok_list()));
    QObject::connect(list_var, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(ok_list(QListWidgetItem*)));
}
  
variableQt::~variableQt()
{}

void variableQt::ok()
{
  
  if(line_var->text() != ""){
    variableQt::sumvars.insert(line_var->text().toStdString());
    vars->at(numVars)->set_name(line_var->text().toStdString());
    numVars++;
    if(numVars >= vars->size()){
      emit closed();
      this->close();
    }else{
      line_var->clear();
      lab_var->setText(QString::fromUtf8(vars->at(numVars)->get_name().c_str()));
      line_var->setFocus();
    }
  }else{
    QMessageBox msgBox;
    msgBox.setText("Le nom de variable ne doit pas être vide");
    msgBox.exec();
  }
}

void variableQt::ok_list(){
  variableQt::sumvars.insert(list_var->currentItem()->text().toStdString());
  vars->at(numVars)->set_name(list_var->currentItem()->text().toStdString());
  list_var->takeItem(list_var->currentRow());
  numVars++;
  if(numVars >= vars->size()){
      emit closed();
      this->close();
    }else{
      line_var->clear();
      lab_var->setText(QString::fromUtf8(vars->at(numVars)->get_name().c_str()));
      line_var->setFocus();
    }
}

void variableQt::keyPressEvent(QKeyEvent* e)
{
  if(e->key() == Qt::Key_Return){
    ok();
  }
}

void variableQt::ok_list(QListWidgetItem*)
{
  ok_list();
}


#include "variableQt.moc"