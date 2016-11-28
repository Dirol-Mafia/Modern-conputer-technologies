#include <MainWindow.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    createActions();
    createMenus();

    selectedItemsCount = 0; //number of selected images in imagesViewer

    model = new ImageProvider("../Test/DB_Lectures");
    filteredModel = new MySortFilterProxyModel(this);
    filteredModel->setSourceModel(model);

    renderCategoriesLayout();
    renderImagesLayout();


    mainLayout = new QHBoxLayout;
    mainLayout->addLayout(dataLayout);
    mainLayout->addLayout(imagesLayout);

    QObject::connect(treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(showImages(const QModelIndex &)));
    QObject::connect(imagesView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(setEnableButtons()));
    QObject::connect(printButton, SIGNAL(clicked()), this, SLOT(callPrinter()));
    this->setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(mainLayout);

    on_treeView_customContextMenuRequested();
}

void MainWindow::renderCategoriesLayout()
{
    treeView = new QTreeView;
    treeView->setModel(filteredModel);
    dataLayout = new QVBoxLayout;
    dataLayout->addWidget(treeView);
}

void MainWindow::renderImagesLayout()
{
    imagesView = new QListView;
    imagesView->setModel(model);

    imagesLayout = new QVBoxLayout;
    editButton = new QPushButton("Редактировать");
    printButton = new QPushButton("Печатать");
    editButton->setEnabled(false);
    printButton->setEnabled(false);
    imagesLayout->addWidget(imagesView);
    imagesLayout->addWidget(editButton);
    imagesLayout->addWidget(printButton);
}

void MainWindow::setEnableButtons()
{
    DataWrapper* paragraphData = static_cast<DataWrapper *>(currentParagraphIndex.internalPointer());
    int imagesCount = paragraphData->children.count();
    selectedItemsCount = 0;

    for (int i = 0; i < imagesCount; ++i)
    {
        if (paragraphData->children[i]->isChecked) ++selectedItemsCount;
    }
    editButton->setEnabled(selectedItemsCount == 1 ? true: false);
    printButton->setEnabled(selectedItemsCount > 0 ? true: false);
}

void MainWindow::showImages(const QModelIndex &proxyIndex)
{
    QModelIndex realIndex = filteredModel->mapToSource(proxyIndex);
    DataWrapper* data = static_cast<DataWrapper *>(realIndex.internalPointer());
    if (data->type == PARAGRAPH)
    {
        currentParagraphIndex = realIndex;
        imagesView->setRootIndex(realIndex);
    }
}

void MainWindow::callPrinter()
{
    DataWrapper* data = static_cast<DataWrapper *>(currentParagraphIndex.internalPointer());

    dialog = new QPrintDialog(&printer);
    dialog->setWindowTitle("Print images");

    if (dialog->exec() == QDialog::Accepted)
    {
        painter.begin(&printer);

        for (int i = 0; i < selectedItemsCount; ++i)
        {
            QString imagePath = static_cast<IData*>(data->children[i]->data)->path;
            QImage currentImage(imagePath);
            QRect rect(currentImage.rect());
            QRect devRect(0, 0, painter.device()->width(), painter.device()->height());
            rect.moveCenter(devRect.center());
            painter.drawImage(rect.topLeft(), currentImage);
            printer.newPage();
        }
        painter.end();
    }
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

const DataWrapper* MainWindow::itemData()
{
  QModelIndex cur_ind = filteredModel->mapToSource(treeView->selectionModel()->currentIndex());
  const DataWrapper* child = static_cast<const DataWrapper*>(cur_ind.internalPointer());
  return child;
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

    QAction* actionEdit = new QAction(tr("Редактировать..."), this);
    connect(actionEdit, &QAction::triggered, this, &MainWindow::editCategory);
    treeView->addAction(actionEdit);

    QAction* actionAdd = new QAction(tr("Добавить..."), treeView);
    connect(actionAdd, &QAction::triggered, this, &MainWindow::addingAction);
    treeView->addAction(actionAdd);

    QAction* actionDelete = new QAction(tr("Удалить..."), treeView);
    connect(actionDelete, &QAction::triggered, this, &MainWindow::deleteAction);
    treeView->addAction(actionDelete);
}

/* Actions for the Context Menu */

void MainWindow::emptyAction()
{
  QMessageBox::information(treeView, "Ups...", "This action is kinda in developing!");
  qDebug() << treeView->selectionModel()->currentIndex().isValid();
  const DataWrapper* child = this->itemData();
  qDebug() << "Selection type: " << child->type;
  switch (child->type) {
      case ROOT:
      case TERM:
      case SUBJECT:
      case THEME:
      case PARAGRAPH:
          qDebug() << static_cast<HData*>(child->data)->name;
          break;
      case IMAGE:
          qDebug() << static_cast<IData*>(child->data)->comment;
          break;
      default:
        break;
    }
}

void MainWindow::editCategory()
{
  const DataWrapper* child = this->itemData();
  QString child_data;
  QString child_comment;
  QString child_tags;
  QString editWhat = getCatName(child->type);

  switch (child->type) {
    case ROOT:
    case TERM:
    case SUBJECT:
    case THEME:
    case PARAGRAPH:
      child_data = static_cast<HData*>(child->data)->name;
      child_comment = static_cast<HData*>(child->data)->comment;
      break;
    case IMAGE:
      child_data = static_cast<IData*>(child->data)->path;
      child_comment = static_cast<IData*>(child->data)->comment;
      child_tags = (QString)static_cast<IData*>(child->data)->tags.join(',');
      break;
    default:
      break;
    }

  QWidget *window = new QWidget;
  QFormLayout *formLayout = new QFormLayout;

  QLineEdit *nameEdit = new QLineEdit;
  nameEdit->setText(child_data);
  QLineEdit *commentEdit = new QLineEdit;
  commentEdit->setText(child_comment);
  commentEdit->setFixedHeight(50);
  QPushButton *buttonEdit = new QPushButton("Редактировать");
  buttonEdit->setMaximumWidth(120);
  QPushButton *buttonCancel = new QPushButton("Отмена");
  buttonCancel->setMaximumWidth(120);
  connect(buttonEdit, &QPushButton::clicked, this, &MainWindow::emptyAction);
  connect(buttonCancel, &QPushButton::clicked, window, &QWidget::close);

  formLayout->addRow(tr("&Наименование"), nameEdit);
  formLayout->addRow(tr("&Комментарий"), commentEdit);
  formLayout->setAlignment(buttonCancel, Qt::AlignCenter);

  if (child->type != IMAGE)
      window->setFixedSize(460, 180);
  else {
      QLineEdit *tagEdit = new QLineEdit;
      tagEdit->setText(child_tags);
      tagEdit->setFixedHeight(50);
      formLayout->addRow(tr("&Тэги (через запятую)"), tagEdit);
      window->setFixedSize(460, 230);
    }

  formLayout->addRow(buttonEdit);
  formLayout->addRow(buttonCancel);

  window->setLayout(formLayout);
  window->setWindowTitle("Редактировать" + editWhat);
  window->show();

}

void MainWindow::editLecture()
{
  const DataWrapper* child = static_cast<const DataWrapper*>(treeView->selectionModel()->currentIndex().internalPointer());
}

void MainWindow::deleteAction()
{
  const DataWrapper* child = this->itemData();
  QString child_data;
  QString child_comment;
  QString child_tags;
  QString deleteWhat = getCatName(child->type);

  switch (child->type) {
    case ROOT:
    case TERM:
    case SUBJECT:
    case THEME:
    case PARAGRAPH:
      child_data = static_cast<HData*>(child->data)->name;
      child_comment = static_cast<HData*>(child->data)->comment;
      break;
    case IMAGE:
      child_data = static_cast<IData*>(child->data)->path;
      child_comment = static_cast<IData*>(child->data)->comment;
      child_tags = (QString)static_cast<IData*>(child->data)->tags.join(',');
      break;
    default:
      break;
    }

  QWidget *window = new QWidget;
  QFormLayout *formLayout = new QFormLayout;
  QMessageBox* warn_mess = new QMessageBox;

  QPushButton* buttonYes = new QPushButton("Да");
  QPushButton* buttonNo = new QPushButton("Нет");
 // warn_mess->warning(window, "Предупреждение",
 //      "Вы действительно хотите удалить " + child_data + " со всем его содержимым без возможности восстановления?",
 //      buttonYes, buttonNo);

  QLabel* name = new QLabel;
  name->setText(child_data);
  formLayout->addRow(tr("&Название:"), name);

  QPushButton* buttonDelete = new QPushButton("Удалить");
  buttonDelete->setMaximumWidth(90);
  QPushButton *buttonCancel = new QPushButton("Отмена");
  buttonCancel->setMaximumWidth(90);
  connect(buttonDelete, &QPushButton::clicked, this, &MainWindow::areYouSure);
  connect(buttonCancel, &QPushButton::clicked, window, &QWidget::close);

  formLayout->addRow(buttonDelete);
  formLayout->addRow(buttonCancel);

  window->setLayout(formLayout);
  window->setFixedSize(460, 180);
  window->setWindowTitle("Удалить" + deleteWhat);
  window->show();
}

void MainWindow::areYouSure()
{
  QMessageBox* yes_no = new QMessageBox;
  const QString yes = "Да";
  const QString no = "Ненененене";
  const QString title = "Предупреждение";
  const QString mess = "Вы уверены, что хотите удалить выбранную категорию со всем её содержимым без возможности восстановления?";
  yes_no->setWindowTitle(title);
  yes_no->setText(mess);
  yes_no->setIcon(QMessageBox::Warning);
  yes_no->addButton(yes, QMessageBox::AcceptRole);
  yes_no->addButton(no, QMessageBox::RejectRole);

  //yes_no->exec();
  if (yes_no->exec() == 0)
    emptyAction();

}

void MainWindow::addingAction()
{
  const DataWrapper* child = this->itemData();
  QString child_data;
  QString child_comment;
  QString child_tags;
  QString addWhat = getSubcatName(child->type);

  QWidget *window = new QWidget;
  QFormLayout *formLayout = new QFormLayout;

  QLineEdit *nameAdd= new QLineEdit;
  nameAdd->setText(child_data);
  QLineEdit *commentAdd= new QLineEdit;
  commentAdd->setText(child_comment);
  commentAdd->setFixedHeight(50);
  QPushButton *buttonAdd = new QPushButton("Добавить");
  QPushButton *buttonCancel = new QPushButton("Отмена");
  connect(buttonAdd, &QPushButton::clicked, this, &MainWindow::emptyAction);
  connect(buttonCancel, &QPushButton::clicked, window, &QWidget::close);

  formLayout->addRow(tr("&Наименование"), nameAdd);
  formLayout->addRow(tr("&Комментарий"), commentAdd);

  if (child->type != PARAGRAPH)
      window->setFixedSize(460, 180);
  else {
      QLineEdit *tagAdd = new QLineEdit;
      tagAdd->setText(child_tags);
      tagAdd->setFixedHeight(50);
      formLayout->addRow(tr("&Тэги (через запятую)"), tagAdd);
      window->setFixedSize(460, 230);
    }

  formLayout->addRow(buttonAdd);
  formLayout->addRow(buttonCancel);

  window->setLayout(formLayout);
  window->setWindowTitle("Добавить" + addWhat);
  window->show();
}
