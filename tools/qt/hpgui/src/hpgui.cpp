#include "hpgui.h"
#include "ui_hpgui.h"
#include <QPlainTextEdit>
#include <QDir>

#include "core/string.h"


QPlainTextEdit* _pteCompilerOut=0;

void jle::alarm_msg(const jle::alarm& alarm)
{
   if (_pteCompilerOut==0)
        std::cout << alarm << std::endl;
   else
       _pteCompilerOut->insertPlainText(JLE_SS(alarm).c_str());
}


Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::WidgetClass), ast_root("")
{
    ui->setupUi(this);

    //  configure context menus  ---------------------------------
    ui->pteGramarSource->addAction(ui->actionCompile_gramar);
    ui->pteGramarSource->addAction(ui->actionConvert_to_c_code);

    ui->pteInput->addAction(ui->actionParse);

    ui->pteRe->addAction        (ui->actionRun_Regular_Expresion);
    ui->pteReInput->addAction   (ui->actionRun_Regular_Expresion);


    //  messages from system
    _pteCompilerOut = ui->pteCompilerOut;

    // code highlight
    highlighter = new Highlighter(ui->pteGramarSource->document());


    //  testing
}

Widget::~Widget()
{
    delete ui;
}


//-------------------------------------------------------------------------------------------------
//      COMPILE GRAMAR


QString sup_compile_gramar(jle::Humble_parser&  h_parser, const QString& gramar_source)
{
    QString result = "Compiling...\n";

    //  compilar
    bool result_bool;
    std::string result_string;
    std::tie(result_bool, result_string) = h_parser.load_rules_from_string(gramar_source.toStdString());
    result.append(result_string.c_str());
    if (result_bool==false)
        throw (result.append("ERRORS"));
    else
        result.append("\nCompiling END (ok)");

    return result;
}

QString sup_gramar_save(const QString& gramarName, const QString& gramar_source)
{
    QString result;

    if(gramarName!="")
    {
        QString name = gramarName;
        if (name.indexOf(".gram")==-1)
            name.append(".gram");
        QFile file (QString("./projects/").append(name));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ))
            throw result.append("Cannot save on... ").append(name);
        if (file.write(gramar_source.toUtf8())  == -1)
            throw result.append("Error writting");
    }
    else
        throw result.append("No file name to save gramar rules");

    return result.append("\n saved ok\n");;
}


void Widget::on_actionCompile_gramar_triggered()
{
    QString result;
    ui->pteCompilerOut->clear();

    try
    {
        ui->twMain->setCurrentIndex(1);   ui->twGramarInfo->setCurrentIndex(0);

        result.append(        sup_gramar_save         (ui->cbProjects->currentText(),   ui->pteGramarSource->toPlainText()));
        result.append(        sup_compile_gramar      (h_parser,                        ui->pteGramarSource->toPlainText()));
    }
    catch(const QString& error)
    {
        ui->pteCompilerOut->appendPlainText(error);
        return;
    }
    ui->pteCompilerOut->appendPlainText(result);

    on_pbLoadSymbols_clicked();
    on_actionParse_triggered();
}




//-------------------------------------------------------------------------------------------------
//      PARSE INPUT


QString sup_save_parse_input(const QString& gramar_name, const QString& input_text)
{
    QString result;

    //  guardar input test
    if(gramar_name!="")
    {
        QString name = gramar_name;
        if (name.indexOf(".gram")==-1)
            name.append(".test");
        else
            name = name.left(name.size()-5).append(".test");
        QFile file (QString("./projects/").append(name));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
            throw result.append("Cannot save on... ").append(name);
        file.write(input_text.toUtf8());
    }
    else
        throw result.append("No file name to input test");

    return result.append("\n saved ok.\n");
}


QString  sup_parse (const QString text2parse, jle::Humble_parser& h_parser, jle::AST_node_item& ast_root)
{
    QString result;

    bool result_bool;
    std::string  result_string;

    std::tie(result_bool, result_string, ast_root) = h_parser.multi_parse(text2parse.toStdString());

    return result.append(result_string.c_str());
}

QString sup_mantein_nodes_ast(const QList<QListWidgetItem *>& selectedNodes, jle::AST_node_item& ast_root)
{
    QString result;

    jle::list<std::string> nodes2mantein;
    foreach(QListWidgetItem *item, selectedNodes)
    {
        nodes2mantein.push_back(item->text().toStdString());
        nodes2mantein.push_back(JLE_SS(item->text().toStdString() << "*"));
    }
    ast_root.remove_nodes_manteining(nodes2mantein);

    return result;
}


void sup_FillAST_listTree(const jle::AST_node_item& node, QTreeWidgetItem* parent_item)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(parent_item);
    item->setText(0, node.name.c_str());
    item->setText(1, node.value.c_str());
    parent_item->addChild(item);
    if (node.next.expired() == false)
        sup_FillAST_listTree(*(node.next), parent_item);
    if (node.down.expired() == false)
        sup_FillAST_listTree(*(node.down), item);
}


QString sup_fill_ast_nodes (QTreeWidget* tw, const jle::AST_node_item& ast_root)
{
    QString result;

    tw->clear();
    QTreeWidgetItem* item = new QTreeWidgetItem(tw);
    item->setText(0, "ROOT");
    sup_FillAST_listTree(ast_root, item);
    tw->expandAll();

    return result;
}


void Widget::on_actionParse_triggered()
{
    QString result;
    ui->pteParsingOut->clear();
    ui->twMain->setCurrentIndex(2);
    try
    {

        ui->twInput->setCurrentIndex(0);
        result.append(      sup_save_parse_input    (ui->cbProjects->currentText(),     ui->pteInput->toPlainText()             ));
        result.append(      sup_parse               (ui->pteInput->toPlainText(),       h_parser,                        ast_root ));

        ui->pteParsed->setPlainText(ast_root.value.c_str());

        result.append(      sup_mantein_nodes_ast   (ui->lwSymbols->selectedItems(),    ast_root                                 ));

        if (ui->cbONCompileGramatTestGotoOutput->isChecked())
            ui->twInput->setCurrentIndex(2);
        else
            ui->twInput->setCurrentIndex(1);
        result.append(      sup_fill_ast_nodes      (ui->twAST_parsed,                  ast_root                                 ));
    }
    catch(const QString& error)
    {
        ui->pteParsingOut->appendPlainText(error);
        return;
    }
    ui->pteParsingOut->appendPlainText(result);

}





//-------------------------------------------------------------------------------------------------


void Widget::on_pbLoadSymbols_clicked()
{
    ui->lwSymbols->clear();

    std::list<std::string> ls;

    ls = h_parser.get_non_terminal_rules();
    foreach(std::string s, ls)
        ui->lwSymbols->addItem(s.c_str());

    ls = h_parser.get_terminal_rules();
    foreach(std::string s, ls)
        ui->lwSymbols->addItem(s.c_str());

    ui->lwSymbols->selectAll();
}



//-------------------------------------------------------------------------------------------------

void Widget::on_pbSelectAllRules_clicked()
{
    ui->lwSymbols->selectAll();
}

void Widget::on_pteGramarSource_selectionChanged()
{
}

//-------------------------------------------------------------------------------------------------

void Widget::on_pbProjectsReload_clicked()
{
    QDir dir("./projects");
    QStringList filters;
    filters << "*.gram";
    ui->cbProjects->clear();
    ui->cbProjects->addItems(dir.entryList(filters));
}

//-------------------------------------------------------------------------------------------------

void Widget::on_cbProjects_currentIndexChanged(int index)
{
    //  leer de ficheros input test
    if(ui->cbProjects->currentText()!="")
    {
        QString origName = ui->cbProjects->currentText();
        if (origName.indexOf(".gram")!=-1)
            origName = origName.left(origName.size()-5);

        {
            QString name = origName;
            QFile file (JLE_SS("./projects/" << name.append(".gram").toStdString()).c_str());
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                ui->pteLoadInfo->setPlainText(JLE_SS("Cannot open on... " << name.toStdString()).c_str());
                return;
            }
            ui->pteGramarSource->setPlainText(file.readAll());
        }

        {
            QString name = origName;
            QFile file (JLE_SS("./projects/" << name.append(".test").toStdString()).c_str());
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                ui->pteLoadInfo->setPlainText(JLE_SS("Cannot open on... " << name.toStdString()).c_str());
                return;
            }
            ui->pteInput->setPlainText(file.readAll());
        }


    }
    else
        ui->pteCompilerOut->appendPlainText("No file name to input test");
    ui->pteLoadInfo->appendPlainText(JLE_SS("Loaded project " << ui->cbProjects->currentText().toStdString() << " ok").c_str());
}

//-------------------------------------------------------------------------------------------------

void Widget::on_actionRun_Regular_Expresion_triggered()
{
    ui->pteReOut->clear();
/*
    mtk::RegExp re (ui->pteRe->toPlainText().toStdString());

    if (re.Match(ui->pteReInput->toPlainText().toStdString()))
    {
        ui->pteReOut->appendPlainText("Match ok.\n");
        ui->pteReOut->appendPlainText(QString("Number of groups... ").append(QString::number(re.GetCount())));
        for (int i=0; i< re.GetCount(); ++i)
            ui->pteReOut->appendPlainText( QString("Group ").append(QString::number(i)).append("  ").append(re.GetString(i).c_str()));
    }
    else
        ui->pteReOut->appendPlainText("it doesn't macth  ;-(");
*/
}
