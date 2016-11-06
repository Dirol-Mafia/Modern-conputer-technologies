#include <MainWindow.h>

MainWindow::MainWindow(QWidget *parent) : QDialog(parent)
{
    createActions();
    createMenus();

    model = new ImageProvider("../Test/DB_Lectures");
    view = new QTreeView;
    view->setModel(model);
    view->setFixedSize(320, 320);

    dataLayout = new QVBoxLayout;
    dataLayout->addWidget(view);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(menuLayout);
    mainLayout->addLayout(dataLayout);

    setFixedSize(512, 512);
    setLayout(mainLayout);
}

void MainWindow::createActions()
{
    addCategory = new QAction(tr("&Категорию"), this);
    addCategory->setStatusTip("Добавить новую категорию");
    connect(addCategory, &QAction::triggered, this, &MainWindow::addCategoryToDb);

    addLecture = new QAction(tr("&Фото лекций"), this);
    addLecture->setStatusTip("Добавить фото лекций");
    connect(addLecture, &QAction::triggered, this, &MainWindow::addLectureToDb);
}

void MainWindow::createMenus()
{
    menuBar = new QMenuBar(this);
    menuEdit = new QMenu("Редактировать");
    menuAdd = new QMenu("Добавить");
    menuEdit->addAction("test1");
    menuEdit->addSeparator();
    menuEdit->addAction("test2");
    menuAdd->addAction(addCategory);
    menuAdd->addAction("Фотографии лекций");
    menuBar->addMenu(menuEdit);
    menuBar->addMenu(menuAdd);
    menuBar->setNativeMenuBar(false);
    menuLayout = new QVBoxLayout;

    menuLayout->setMenuBar(menuBar);
}

void MainWindow::addLectureToDb()
{

}

void MainWindow::addCategoryToDb()
{
    QWidget *window = new QWidget;
    QFormLayout *formLayout = new QFormLayout;

    QLabel *text = new QLabel("Введите данные");
    QLineEdit *nameEdit = new QLineEdit;
    QLineEdit *commentEdit = new QLineEdit;
    commentEdit->setFixedHeight(50);
    QPushButton *button = new QPushButton("Добавить");

    QGroupBox *groupBox = new QGroupBox;
    QRadioButton *isSemester = new QRadioButton("Семестр");
    QRadioButton *isSubject = new QRadioButton("Предмет");
    QRadioButton *isTheme = new QRadioButton("Тема");
    QVBoxLayout *vbox = new QVBoxLayout;
    isSemester->setChecked(true);
    vbox->addWidget(isSemester);
    vbox->addWidget(isSubject);
    vbox->addWidget(isTheme);
    groupBox->setLayout(vbox);

    formLayout->addRow(tr("&Наименование"), nameEdit);
    formLayout->addRow(tr("&Комментарий"), commentEdit);
    formLayout->addRow(tr("&Тип"), groupBox);
    formLayout->addRow(button);

    window->setLayout(formLayout);
    window->setWindowTitle("Добавить категорию");
    window->setFixedSize(460, 320);
    window->show();
    //QObject::connect(button, SIGNAL(clicked());
}
