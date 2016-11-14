#include <MainWindow.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    createActions();
    createMenus();

    model = new ImageProvider("../Test/DB_Lectures");
    filteredModel = new MySortFilterProxyModel(this);
    filteredModel->setSourceModel(model);

    treeView = new QTreeView;
    treeView->setModel(filteredModel);
    dataLayout = new QVBoxLayout;
    dataLayout->addWidget(treeView);

    listView = new QTreeView;
    listView->setModel(model);

    imagesLayout = new QVBoxLayout;
    editButton = new QPushButton("Редактировать");
    printButton = new QPushButton("Печатать");
    editButton->setEnabled(false);
    printButton->setEnabled(false);
    imagesLayout->addWidget(listView);
    imagesLayout->addWidget(editButton);
    imagesLayout->addWidget(printButton);

    mainLayout = new QHBoxLayout;
    mainLayout->addLayout(dataLayout);
    mainLayout->addLayout(imagesLayout);

    QObject::connect(treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(showImages(const QModelIndex &)));

    this->setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(mainLayout);

    on_treeView_customContextMenuRequested();
}

void MainWindow::showImages(const QModelIndex &proxyIndex)
{
    QModelIndex realIndex = filteredModel->mapToSource(proxyIndex);
    qDebug() << "Real index: " << realIndex;
    listView->setRootIndex(realIndex);
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

    this->setMenuBar(menuBar);
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
}

void MainWindow::on_treeView_customContextMenuRequested()
{
    treeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    QAction* actionEdit = new QAction(tr("Редактировать"), this);
    connect(actionEdit, &QAction::triggered, this, &MainWindow::emptyAction);
    treeView->addAction(actionEdit);

    QAction* actionAdd = new QAction(tr("Добавить подкатегорию"), treeView);
    connect(actionAdd, &QAction::triggered, this, &MainWindow::emptyAction);
    treeView->addAction(actionAdd);

    QAction* actionDelete = new QAction(tr("Удалить категорию"), treeView);
    connect(actionDelete, &QAction::triggered, this, &MainWindow::emptyAction);
    treeView->addAction(actionDelete);

    /*QAction* actionEdit = new QAction(tr("Редактировать"), this);
    QMenu *contextMenu = new QMenu(view);
    contextMenu->setTitle(tr("Открыть"));
    contextMenu->addAction(actionEdit);
    contextMenu->exec(QCursor::pos());*/
}

/* Actions for the Context Menu */

void MainWindow::emptyAction()
{
  QMessageBox::information(treeView, "Ups...", "This action is kinda in developing!");
}

void MainWindow::editCategory()
{

}

void MainWindow::editLecture(const QModelIndex& index)
{

}
