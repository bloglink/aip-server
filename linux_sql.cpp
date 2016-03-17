#include "linux_sql.h"

LINUX_SQL::LINUX_SQL(QWidget *parent) :
    QWidget(parent)
{
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.11.21
  * brief:      获取最大ID
******************************************************************************/
int LINUX_SQL::selectMax(QVariant table)
{
    int max = 0;
    QSqlQuery query(db);
    QString str = "select max(id) from :table";
    str.replace(":table",table.toString());
    if (!query.exec(str)){
        qDebug()<<query.lastError();
    }else{
        while(query.next())
            max = query.value(0).toInt();
    }
    return max;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.11.21
  * brief:      打开数据库
******************************************************************************/
bool LINUX_SQL::createConnetion(QVariant sql)
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");

    QFile file(sql.toString());
    if (file.exists()){
        db.setDatabaseName(sql.toString());
    }else{
        qDebug()<<"file not exist";
        return false;
    }

    if (!db.open()){
        qDebug()<<db.lastError();
        return false;
    }else{
        return true;
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.11.21
  * brief:      关闭数据库
******************************************************************************/
bool LINUX_SQL::close_connection()
{
    db.close();
    return true;
}
/******************************************************************************
                                    END
******************************************************************************/
