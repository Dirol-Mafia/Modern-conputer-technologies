#include <MainWindow.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    createActions();
    selectedImagesCount = 0;
    model = new ImageProvider("../Test/DB_Lectures");
    filteredModel = new MySortFilterProxyModel(this);
    filteredModel->setSourceModel(model);

    renderCategoriesLayout();
    renderImagesLayout();

    mainLayout = new QHBoxLayout;
    mainLayout->addLayout(dataLayout);
    mainLayout->addLayout(imagesLayout);

    QObject::connect(treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(showImages(const QModelIndex &)));
    QObject::connect(imagesView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onImageClick()));
    QObject::connect(printButton, SIGNAL(clicked()), this, SLOT(callPrinter()));
    QObject::connect(editButton, SIGNAL(clicked()), this, SLOT(callEditForm()));
    this->setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(mainLayout);

    on_treeView_customContextMenuRequested();
    createMenus();
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
    editButton = new QPushButton("Подготовить к печати");
    printButton = new QPushButton("Печатать");
    editButton->setEnabled(false);
    printButton->setEnabled(false);
    imagesLayout->addWidget(imagesView);
    imagesLayout->addWidget(editButton);
    imagesLayout->addWidget(printButton);
}

void MainWindow::onImageClick()
{
    DataWrapper* paragraphData = static_cast<DataWrapper *>(currentParagraphIndex.internalPointer());
    int imagesCount = paragraphData->children.count();
    selectedImagesCount = 0;
    for (int i = 0; i < imagesCount; ++i)
    {
        if (paragraphData->children[i]->isChecked)
        {
            ++selectedImagesCount;
            selectedImages[i] = true;
        }
        else
        {
            selectedImages[i] = false;
        }
    }
    setEnableButtons();
}

void MainWindow::setEnableButtons()
{
    editButton->setEnabled(selectedImagesCount == 1 ? true: false);
    printButton->setEnabled(selectedImagesCount > 0 ? true: false);
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
    selectedImages.fill(false, data->children.count());
    selectedImagesCount = 0;
}

void MainWindow::callPrinter()
{
    printer.setOrientation(QPrinter::Portrait);
    dialog = new QPrintPreviewDialog(&printer);
    connect(dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(drawImagesOnSheet(QPrinter*)));
    dialog->exec();
}

void MainWindow::drawImagesOnSheet(QPrinter* printer)
{
    DataWrapper* data = static_cast<DataWrapper *>(currentParagraphIndex.internalPointer());
    painter.begin(printer);
    int lastPage = selectedImagesCount - 1;
    for (int i = 0; i < selectedImagesCount; ++i)
    {
        QString imagePath = static_cast<IData*>(data->children[i]->data)->path;
        QImage currentImage(imagePath);
        QImage scaledImage = currentImage.scaled(640, 640, Qt::KeepAspectRatio);
        QRect rect(scaledImage.rect());
        QRect devRect(0, 0, painter.device()->width(), painter.device()->height());
        rect.moveCenter(devRect.center());
        painter.drawImage(rect.topLeft(), scaledImage);
        if (i != lastPage) printer->newPage();
    }
    painter.end();
}

void MainWindow::callEditForm()
{
    DataWrapper* data = static_cast<DataWrapper *>(currentParagraphIndex.internalPointer());
    int i = 0;
    //TODO порефакторить этот момент, пока очень костыльно
    while (i < selectedImages.count() && !selectedImages[i])
    {
        ++i;
    }

    QString imagePath = static_cast<IData*>(data->children[i]->data)->path;
    ImageEditForm *editForm = new ImageEditForm(imagePath);
    editForm->show();
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
    menuEdit = new QMenu("Редактирование категорий");
    menuAdd = new QMenu("Редактирование лекций");

    menuEdit->addAction(actionEdit);
    menuEdit->addSeparator();
    menuEdit->addAction(actionAdd);
    menuEdit->addSeparator();
    menuEdit->addAction(actionDelete);

    //menuAdd->addAction(addCategory);
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

    actionEdit = new QAction(tr("Редактировать..."), this);
    connect(actionEdit, &QAction::triggered, this, &MainWindow::editCategory);
    treeView->addAction(actionEdit);

    actionAdd = new QAction(tr("Добавить..."), treeView);
    connect(actionAdd, &QAction::triggered, this, &MainWindow::addingAction);
    treeView->addAction(actionAdd);

    actionDelete = new QAction(tr("Удалить..."), treeView);
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

  editWindow = new QWidget;
  editLayout = new QFormLayout;

  nameEdit = new QLineEdit;
  nameEdit->setText(child_data);
  commentEdit = new QLineEdit;
  commentEdit->setText(child_comment);
  commentEdit->setFixedHeight(50);
  QPushButton *buttonEdit = new QPushButton("Редактировать");
  QPushButton *buttonCancel = new QPushButton("Отмена");
  connect(buttonEdit, &QPushButton::clicked, this, &MainWindow::edit);
  connect(buttonCancel, &QPushButton::clicked, editWindow, &QWidget::close);

  editLayout->addRow(tr("&Наименование"), nameEdit);
  editLayout->addRow(tr("&Комментарий"), commentEdit);
  editLayout->setAlignment(buttonCancel, Qt::AlignCenter);

  if (child->type != IMAGE)
      editWindow->setFixedSize(460, 180);
  else {
      tagEdit = new QLineEdit;
      tagEdit->setText(child_tags);
      tagEdit->setFixedHeight(50);
      editLayout->addRow(tr("&Тэги (через запятую)"), tagEdit);
      editWindow->setFixedSize(460, 230);
    }

  editLayout->addRow(buttonEdit);
  editLayout->addRow(buttonCancel);

  editWindow->setLayout(editLayout);
  editWindow->setWindowTitle("Редактировать" + editWhat);
  editWindow->show();

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

  deleteWindow = new QWidget;
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
  QPushButton *buttonCancel = new QPushButton("Отмена");
  connect(buttonDelete, &QPushButton::clicked, this, &MainWindow::areYouSure);
  connect(buttonCancel, &QPushButton::clicked, deleteWindow, &QWidget::close);

  formLayout->addRow(buttonDelete);
  formLayout->addRow(buttonCancel);

  deleteWindow->setLayout(formLayout);
  deleteWindow->setFixedSize(460, 180);
  deleteWindow->setWindowTitle("Удалить" + deleteWhat);
  deleteWindow->show();
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
    remove();
}

void MainWindow::addingAction()
{
  const DataWrapper* child = this->itemData();
  QString child_data;
  QString child_comment;
  QString child_tags;
  QString addWhat = getSubcatName(child->type);

  addWindow = new QWidget;
  addLayout = new QFormLayout;

  if (child->type != PARAGRAPH){
    nameAdd= new QLineEdit;
    nameAdd->setText(child_data);
    commentAdd = new QLineEdit;
    commentAdd->setText(child_comment);
    commentAdd->setFixedHeight(50);
    QPushButton *buttonAdd = new QPushButton("Добавить");
    QPushButton *buttonCancel = new QPushButton("Отмена");
    connect(buttonAdd, &QPushButton::clicked, this, &MainWindow::add);
    connect(buttonCancel, &QPushButton::clicked, addWindow, &QWidget::close);

    addLayout->addRow(tr("&Наименование"), nameAdd);
    addLayout->addRow(tr("&Комментарий"), commentAdd);

    addLayout->addRow(buttonAdd);
    addLayout->addRow(buttonCancel);

    addWindow->setLayout(addLayout);
    addWindow->setFixedSize(460, 180);
    }
  else {
      QPushButton* buttonReview = new QPushButton("Выбрать сканы...");
      connect(buttonReview, &QPushButton::clicked, this, &MainWindow::addLectures);
      addLayout->addRow(buttonReview);
      addWindow->setLayout(addLayout);
      addWindow->setFixedSize(250, 50);
    }
  addWindow->setWindowTitle("Добавить" + addWhat);
  addWindow->show();
}

// Actions for context menu (editing model)
void MainWindow::add()
{
  const DataWrapper* parent = this->itemData();

  QModelIndex parent_ind = filteredModel->mapToSource(treeView->selectionModel()->currentIndex());
  parent = model->dataForIndex(parent_ind);
  int p_id = parent->id;
  int row_count = parent->count;
  //int ins_row = parent_ind.row() + count;

  if (!model->insertRows(row_count, 1,parent_ind))
    return;

  model->fetchAll(parent_ind);
  QModelIndex child = model->index(row_count, 0, parent_ind);
  DataWrapper* child_data = model->dataForIndex(child);
  HData add_data = {(h_type)(1 + (int)parent->type), nameAdd->text(), commentAdd->text()};
  QVariant add_data_qvariant = QVariant::fromValue(add_data);

  if(!model->setData(child, add_data_qvariant, Qt::EditRole))
    return;
  else{
    qDebug() << "Data was Set!!!!!";
    QMessageBox::information(treeView, "OK", "Успешно!");
    treeView->update(parent_ind);
    }

  updateActions();
  //treeView->update();
}

void MainWindow::edit()
{
  QModelIndex child = filteredModel->mapToSource(treeView->selectionModel()->currentIndex());
  DataWrapper* child_data_wrapper = model->dataForIndex(child);
  HData* child_data = static_cast<HData*>(child_data_wrapper->data);
  if (child_data->name == nameEdit->text() && child_data->comment == commentEdit->text())
    return;
  HData add_data = {(h_type)(1 + (int)child_data_wrapper->type), nameEdit->text(), commentEdit->text()};
  QVariant add_data_qvariant = QVariant::fromValue(add_data);

  if(!model->setData(child, add_data_qvariant, Qt::EditRole))
    return;
  else{
    qDebug() << "Data was Set!!!!!";
    treeView->update(child);
    QMessageBox::information(treeView, "OK", "Успешно!");
    editWindow->close();
    }
}

void MainWindow::remove()
{
  QModelIndex child = filteredModel->mapToSource(treeView->selectionModel()->currentIndex());
  QModelIndex parent_ind = model->parent(child);
  DataWrapper* child_data_wrapper = model->dataForIndex(child);
  if(!model->removeRows(child_data_wrapper->number, 1, parent_ind))
    return;
  else{
    QMessageBox::information(treeView, "OK", "Успешно!");
    deleteWindow->close();
    }
}

void MainWindow::updateActions()
{
  bool has_selection = !treeView->selectionModel()->selection().isEmpty();
  actionDelete->setEnabled(has_selection);

  bool has_current = treeView->selectionModel()->currentIndex().isValid();
  actionAdd->setEnabled(has_current);
  actionEdit->setEnabled(has_current);

  if (has_current)
    treeView->closePersistentEditor(treeView->selectionModel()->currentIndex());
}

void MainWindow::addLectures()
{
  picturePaths = QFileDialog::getOpenFileNames(0, "Выбор сканов лекций", "", "*.jpg *.png *.bmp", 0, 0);
  pictureGrid = new QGridLayout;

  size_t num_pics = picturePaths.size();
  pictureComments.reserve(num_pics);
  pictureTags.reserve(num_pics);
  delButtons.reserve(num_pics);
  picLayouts.reserve(num_pics);
  comLayouts.reserve(num_pics);
  tagLayouts.reserve(num_pics);
  butLayouts.reserve(num_pics);
  picLabels.reserve(num_pics);

  size_t rowCount = 0;

  for (auto it = picturePaths.begin(); it != picturePaths.end(); ++it)
    {
      size_t colCount = 0;
      QFormLayout* picLayout = new QFormLayout;
      QFormLayout* comLayout = new QFormLayout;
      QFormLayout* tagLayout = new QFormLayout;
      QFormLayout* buttonLayout = new QFormLayout;

      QPixmap pic(*it);
      //QPixmap scaled = pic.scaledToHeight(100, Qt::FastTransformation).scaledToWidth(200, Qt::KeepAspectRatio);
      QPixmap scaled = pic.scaled(QSize(200, 400), Qt::KeepAspectRatio);
      QLabel *picLabel = new QLabel();
      picLabel->setPixmap(scaled);
      picLayout->addRow(picLabel);
      picLabels.push_back(picLabel);
      picLayouts.push_back(picLayout);
      pictureGrid->addLayout(picLayout, rowCount, colCount++);

      QTextEdit* comLine = new QTextEdit;
      comLine->setFixedHeight(100);
      comLine->setFixedWidth(300);
      comLine->setPlaceholderText("Комментарий");
      comLayout->addRow(comLine);
      comLayouts.push_back(comLayout);
      pictureGrid->addLayout(comLayout, rowCount, colCount++);
      pictureComments.push_back(comLine);

      QTextEdit* tagLine = new QTextEdit;
      tagLine->setFixedHeight(100);
      tagLine->setFixedWidth(300);
      tagLine->setPlaceholderText("Тэги (через запятую)");
      tagLayout->addRow(tagLine);
      tagLayouts.push_back(tagLayout);
      pictureGrid->addLayout(tagLayout, rowCount, colCount++);
      pictureTags.push_back(tagLine);

      QPushButton* deleteButton = new QPushButton("Удалить");
      buttonLayout->addRow(deleteButton);
      connect(deleteButton, &QPushButton::clicked, this, &MainWindow::removePicFromSelection);
      delButtons.push_back(deleteButton);
      butLayouts.push_back(buttonLayout);
      pictureGrid->addLayout(buttonLayout, rowCount, colCount++);

      ++rowCount;
    }

  editPicturesWin = new QWidget;
  editPicturesWin->setLayout(pictureGrid);
  editPicturesWin->setWindowTitle("Добавление сканов");
  editPicturesWin->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  editPicScrollAlrea = new QScrollArea;
  editPicScrollAlrea->setWidget(editPicturesWin);

  editPicScrollAlrea->show();
}

void MainWindow::removePicFromSelection()
{
  for (size_t i = 0; i < delButtons.size(); ++i)
    if (sender() == delButtons[i]){
        qDebug() << "Sender: " << i;

        pictureGrid->removeItem(picLayouts[i]);
        pictureGrid->removeItem(comLayouts[i]);
        pictureGrid->removeItem(tagLayouts[i]);
        pictureGrid->removeItem(butLayouts[i]);

        delete picLabels.at(i);
        picLabels.removeAt(i);

        delete pictureComments.at(i);
        pictureComments.removeAt(i);

        picturePaths.removeAt(i);

        delete pictureTags.at(i);
        pictureTags.removeAt(i);

        delete delButtons.at(i);
        delButtons.removeAt(i);

        picLayouts.at(i)->destroyed();
        delete picLayouts.at(i);
        picLayouts.removeAt(i);

        delete comLayouts.at(i);
        comLayouts.removeAt(i);

        delete tagLayouts.at(i);
        tagLayouts.removeAt(i);

        delete butLayouts.at(i);
        butLayouts.removeAt(i);

       /* for (size_t j = i; j < picturePaths.size(); ++j){
            pictureGrid->add
          }*/

        pictureGrid->update();
        editPicturesWin->repaint();
        editPicturesWin->update();
        editPicScrollAlrea->repaint();
        editPicScrollAlrea->update();

        break;
      }
}
