#include <MainWindow.h>

#define COMMENT_BOLD tr("<b>Комментарий: </b>")
#define TAGS_BOLD tr("<b>Тэги: </b>")
#define DEFAULT_COMM_CAT_ITALIC tr("<i>сначала выберите категорию.</i>")
#define DEFAULT_COMM_TAGS_IMG_ITALIC tr("<i>сначала выберите изображение.</i>")

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    selectedImagesCount = 0;
    addSemester = false;
    model = new ImageProvider("../Test/DB_Lectures");
    filteredModel = new MySortFilterProxyModel(this);
    filteredModel->setSourceModel(model);

    toolbar = new QToolBar;

    renderCategoriesLayout();
    renderImagesLayout();
    renderToolbar();
    createActions();

    mainLayout = new QHBoxLayout;
    mainLayout->addLayout(dataLayout);
    mainLayout->addLayout(imagesLayout);

    QObject::connect(treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(showImages(const QModelIndex &)));
    QObject::connect(treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(showComments(const QModelIndex &)));
    QObject::connect(imagesView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onImageClick()));
    QObject::connect(imagesView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onImageDoubleClick()));
    QObject::connect(printButton, SIGNAL(clicked()), this, SLOT(callPrinter()));
    QObject::connect(editButton, SIGNAL(clicked()), this, SLOT(callEditForm()));
    QObject::connect(deleteButton, SIGNAL(clicked()), this, SLOT(areYouSureDelPics()));
    QObject::connect(searchButton, SIGNAL(clicked()), this, SLOT(onSearchButtonClick()));
    QObject::connect(addButton, &QPushButton::clicked, this, &MainWindow::addingAction);
    QObject::connect(newSemester,&QPushButton::clicked, this, &MainWindow::addNewSemester);
    QObject::connect(selectAllCheckBox,SIGNAL(toggled(bool)), this, SLOT(selectAllImages()));
    this->setCentralWidget(new QWidget(this));

    this->addToolBar(toolbar);
    centralWidget()->setLayout(mainLayout);

    on_treeView_customContextMenuRequested();

    setWindowTitle("Приложение для просмотра и сортировки лекций");

    createMenus();
}

void MainWindow::renderToolbar()
{
    searchInput = new QLineEdit;
    searchInput->setText("");
    searchInput->setFixedWidth(250);
    searchInput->setFixedHeight(25);
    searchButton = new QPushButton("Найти");
    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    toolbar->addWidget(empty);
    toolbar->addWidget(searchInput);
    toolbar->addWidget(searchButton);

}

void MainWindow::renderCategoriesLayout()
{
    treeView = new QTreeView;
    treeView->setModel(filteredModel);

    dataLayout = new QVBoxLayout;
    dataLayout->addWidget(treeView);

    catComLabel = new QLabel;
    QString comm = COMMENT_BOLD + DEFAULT_COMM_CAT_ITALIC;
    catComLabel->setText(comm);
    catComLabel->setMaximumWidth(500);
    catComLabel->setWordWrap(true);
    dataLayout->addWidget(catComLabel);

    newSemester = new QPushButton("Новый семестр");
    dataLayout->addWidget(newSemester);
}

void MainWindow::renderImagesLayout()
{
    imagesView = new QListView;
    imagesView->setModel(model);
    imagesLayout = new QVBoxLayout;

    addButton = new QPushButton("Добавить");
    editButton = new QPushButton("Подготовить к печати");
    printButton = new QPushButton("Печатать");
    deleteButton = new QPushButton("Удалить");
    selectAllCheckBox = new QCheckBox("Выбрать всё");

    addButton->setEnabled(false);
    editButton->setEnabled(false);
    printButton->setEnabled(false);
    deleteButton->setEnabled(false);
    selectAllCheckBox->setEnabled(false);

    imgComLabel = new QLabel;
    imgComLabel->setText(COMMENT_BOLD + DEFAULT_COMM_TAGS_IMG_ITALIC);
    imgComLabel->setMaximumWidth(500);
    imgComLabel->setWordWrap(true);

    imgTagLabel = new QLabel;
    imgTagLabel->setText(TAGS_BOLD + DEFAULT_COMM_TAGS_IMG_ITALIC);
    imgTagLabel->setMaximumWidth(500);
    imgTagLabel->setWordWrap(true);

    imagesLayout->addWidget(imagesView);
    imagesLayout->addWidget(imgComLabel);
    imagesLayout->addWidget(imgTagLabel);

    imagesLayout->addWidget(selectAllCheckBox);
    imagesLayout->addWidget(addButton);
    imagesLayout->addWidget(editButton);
    imagesLayout->addWidget(printButton);
    imagesLayout->addWidget(deleteButton);
}

void MainWindow::showComments(const QModelIndex &index)
{
  QModelIndex realIndex = filteredModel->mapToSource(index);
  DataWrapper* datawrapper = static_cast<DataWrapper*>(realIndex.internalPointer());
  HData* data = static_cast<HData*>(datawrapper->data);
  QString comment = data->comment;
  if (comment == "")
      comment = "<i>без комментариев.</i>";
  catComLabel->setText(COMMENT_BOLD + comment);
}

void MainWindow::onImageClick()
{
    getSelectedItems();
    DataWrapper* pictureData = static_cast<DataWrapper *>(imagesView->selectionModel()->currentIndex().internalPointer());
    if (pictureData)
    {
        IData* child_data = static_cast<IData*>(pictureData->data);
        QString comm = child_data->comment;
        QString tags = child_data->tags.join(",");

        if (comm == "")
            comm = "<i>без комментариев.</i>";
        if (tags == "")
            tags = "<i>не назначены.</i>";

        imgComLabel->setText("<b>Комментарий: </b>" + comm);
        imgTagLabel->setText("<b>Тэги: </b>" + tags);
    }
    setEnableButtons();
}

void MainWindow::getSelectedItems()
{    
    DataWrapper* paragraphData = static_cast<DataWrapper *>(currentParagraphIndex.internalPointer());
    int imagesCount = paragraphData->children.count();
    if (selectedImages.size() < paragraphData->count){
        selectedImages.clear();
        selectedImages.resize(paragraphData->count);
    }
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
}


void MainWindow::onImageDoubleClick()
{
    DataWrapper* paragraphData = static_cast<DataWrapper *>(currentParagraphIndex.internalPointer());
    DataWrapper* pictureData = static_cast<DataWrapper *>(imagesView->selectionModel()->currentIndex().internalPointer());
    ImageViewer* img_vwr = new ImageViewer(pictureData->number, paragraphData, "");
    img_vwr->show();
}

void MainWindow::setEnableButtons()
{
    editButton->setEnabled(selectedImagesCount == 1);
    editPic->setEnabled(selectedImagesCount == 1);
    actionEditLect->setEnabled(imagesView->selectionModel()->currentIndex().internalPointer() != nullptr);

    printButton->setEnabled(selectedImagesCount > 0);
    print->setEnabled(selectedImagesCount > 0);

    deleteButton->setEnabled(selectedImagesCount > 0);
    actionDeleteLect->setEnabled(selectedImagesCount > 0);
}

void MainWindow::selectAllImages()
{
    DataWrapper* data = static_cast<DataWrapper *>(currentParagraphIndex.internalPointer());
    int childrenCount = data->children.count();
    for (int i = 0; i < childrenCount; ++i)
    {
        QModelIndex child = model->index(i, 0, currentParagraphIndex);
        DataWrapper* child_data_wrapper = model->dataForIndex(child);
        IData* child_data = static_cast<IData*>(child_data_wrapper->data);
        QVariant add_data_qvariant = QVariant::fromValue(*child_data);
        model->setChecked(child, add_data_qvariant, Qt::CheckStateRole, selectAllCheckBox->isChecked());
        //model->setData(child, add_data_qvariant, Qt::CheckStateRole);
        imagesView->update(child);
    }
    getSelectedItems();
    setEnableButtons();
}

void MainWindow::onSearchButtonClick()
{
    if (searchInput->text().length() > 0)
            searchByTags();
}

void MainWindow::searchByTags()
{
    QStringList paths;
    QSqlQuery query;
    QStringList tags = searchInput->text().split(",");
    for (int i = 0; i < tags.size(); ++i)
    {
        tags[i] = tags[i].trimmed();
        query.prepare("SELECT File_name from lectures WHERE Tags LIKE ?");
        query.addBindValue("%" + tags[i] + "%");
        query.exec();
        query.next();
        do
        {
            QString result = query.value(0).toString();
            if (result.length() > 0)
                paths.push_back(result);
        }
        while (query.next());
    }
    if (paths.size() > 0)
    {
        ImagesWithTags *imagesWithTagsForm = new ImagesWithTags(paths);
        imagesWithTagsForm->show();
    }
    else
        noSuchTags();
}

void MainWindow::lineEditKeyPressed()
{
    qDebug() << "Key pressed!";
}

void MainWindow::noSuchTags()
{
    QMessageBox* noTags = new QMessageBox;
    const QString mess = "Упс! Изображений с такими тегами нет. Попробуй еще раз!";
    noTags->setWindowTitle("Нет таких тегов");
    noTags->setText(mess);
    noTags->setIcon(QMessageBox::Warning);
    noTags->addButton("OK", QMessageBox::AcceptRole);

    noTags->exec();
}

void MainWindow::addNewSemester()
{
    addSemester = true;
    addingAction();
}

void MainWindow::showImages(const QModelIndex &proxyIndex)
{
    QModelIndex realIndex = filteredModel->mapToSource(proxyIndex);
    DataWrapper* data = static_cast<DataWrapper *>(realIndex.internalPointer());
    if (data && data->type == PARAGRAPH)
    {
        currentParagraphIndex = realIndex;
        imagesView->setRootIndex(realIndex);
        selectedImages.fill(false, data->children.count());
        selectedImagesCount = 0;
    }

    const DataWrapper* data_selected = itemData();
    if (data_selected && data_selected->type == PARAGRAPH)
    {
        addButton->setEnabled(true);
        actionAddLect->setEnabled(true);
        selectAllCheckBox->setEnabled(true);
    }
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
    int currentPage = 0;
    for (int i = 0; i < data->count; ++i)
    {
        if (!data->children.at(i)->isChecked)
            continue;
        QString imagePath = static_cast<IData*>(data->children[i]->data)->path;
        QImage currentImage(imagePath);
        QImage scaledImage = currentImage.scaled(640, 640, Qt::KeepAspectRatio);
        QRect rect(scaledImage.rect());
        QRect devRect(0, 0, painter.device()->width(), painter.device()->height());
        rect.moveCenter(devRect.center());
        painter.drawImage(rect.topLeft(), scaledImage);
        if (currentPage != lastPage) printer->newPage();
        ++currentPage;
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

void MainWindow::areYouSureDelPics()
{
    QMessageBox* yes_no = new QMessageBox;
    const QString yes = "Да";
    const QString no = "Ненененене";
    const QString title = "Предупреждение";
    const QString mess = "Вы уверены, что хотите удалить выбранные изображения без возможности восстановления?";
    yes_no->setWindowTitle(title);
    yes_no->setText(mess);
    yes_no->setIcon(QMessageBox::Warning);
    yes_no->addButton(yes, QMessageBox::AcceptRole);
    yes_no->addButton(no, QMessageBox::RejectRole);

    if (yes_no->exec() == 0)
      removePictures();

}

void MainWindow::createActions()
{

    actionEdit = new QAction(tr("Редактировать..."), this);
    actionEdit->setShortcut(tr("Ctrl+E"));
    connect(actionEdit, &QAction::triggered, this, &MainWindow::editCategory);

    actionAdd = new QAction(tr("Добавить..."), treeView);
    actionAdd->setShortcut(tr("Ctrl+A"));
    connect(actionAdd, &QAction::triggered, this, &MainWindow::addingAction);

    actionDelete = new QAction(tr("Удалить..."), treeView);
    actionDelete->setShortcut(tr("Delete"));
    connect(actionDelete, &QAction::triggered, this, &MainWindow::deleteAction);

    actionEditLect = new QAction(tr("Редактировать..."), imagesView);
    actionEditLect->setEnabled(false);
    connect(actionEditLect, &QAction::triggered, this, &MainWindow::editPictureInfo);

    actionAddLect = new QAction(tr("Добавить..."), imagesView);
    actionAddLect->setEnabled(false);
    connect(actionAddLect,&QAction::triggered, this, &MainWindow::addingAction);

    actionDeleteLect = new QAction(tr("Удалить..."), imagesView);
    actionDeleteLect->setEnabled(false);
    connect(actionDeleteLect, &QAction::triggered, this, &MainWindow::areYouSureDelPics);

    print = new QAction(tr("Печатать..."), imagesView);
    print->setEnabled(false);
    connect(print, &QAction::triggered, this, &MainWindow::callPrinter);

    editPic = new QAction(tr("Подготовить к печати..."), imagesView);
    editPic->setEnabled(false);
    connect(editPic, &QAction::triggered, this, &MainWindow::callEditForm);

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
    menuAdd->addAction(actionAddLect);
    menuAdd->addAction(actionEditLect);
    menuAdd->addAction(editPic);
    menuAdd->addAction(print);
    menuAdd->addAction(actionDeleteLect);

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
    treeView->addAction(actionEdit);
    treeView->addAction(actionAdd);
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
      /*child_data = static_cast<IData*>(child->data)->path;
      child_comment = static_cast<IData*>(child->data)->comment;
      child_tags = (QString)static_cast<IData*>(child->data)->tags.join(',');
      nameEdit->setText(child_data);
      editLayout->addRow(tr("&Путь"), nameEdit);
      editLayout->addRow(buttonBrowse);*/
      break;
    default:
      break;
    }

  editWindow = new QWidget;
  nameEdit = new QLineEdit;
  editLayout = new QFormLayout;

  nameEdit->setText(child_data);
  editLayout->addRow(tr("&Наименование"), nameEdit);

  commentEdit = new QPlainTextEdit;
  commentEdit->appendPlainText(child_comment);
  commentEdit->setFixedHeight(50);
  QPushButton *buttonEdit = new QPushButton("Редактировать");
  QPushButton *buttonCancel = new QPushButton("Отмена");
  connect(buttonEdit, &QPushButton::clicked, this, &MainWindow::edit);
  connect(buttonCancel, &QPushButton::clicked, editWindow, &QWidget::close);

  editLayout->addRow(tr("&Комментарий"), commentEdit);
  editLayout->setAlignment(buttonCancel, Qt::AlignCenter);

  if (child->type != IMAGE)
      editWindow->setFixedSize(460, 180);
  else {
      tagEdit = new QPlainTextEdit;
      tagEdit->appendPlainText(child_tags);
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

void MainWindow::editPictureInfo()
{
    DataWrapper* child = static_cast<DataWrapper*>(imagesView->selectionModel()->currentIndex().internalPointer());
    IData* child_data = static_cast<IData*>(child->data);
    QString path =  child_data->path;
    QString comm = child_data->comment;
    QString tags = child_data->tags.join(",");

    if (nameEdit)
        delete nameEdit;
    nameEdit = new QLineEdit;
    nameEdit->setText(path);
    nameEdit->setFixedHeight(50);

    if (commentEdit)
        delete commentEdit;
    commentEdit = new QPlainTextEdit;
    commentEdit->appendPlainText(comm);
    commentEdit->setFixedHeight(100);

    if (tagEdit)
        delete tagEdit;
    tagEdit = new QPlainTextEdit;
    tagEdit->appendPlainText(tags);
    tagEdit->setFixedHeight(100);

    editPicInfo = new QWidget;
    editLayout = new QFormLayout;

    QPushButton* browseButton = new QPushButton("Обзор...");
    connect(browseButton, &QPushButton::clicked, this, &MainWindow::browsePic);

    QPushButton* editButton = new QPushButton("Редактировать");
    connect(editButton, &QPushButton::clicked, this, &MainWindow::editInfo);

    QPushButton* cancelButton = new QPushButton("Отмена");
    connect(cancelButton, &QPushButton::clicked, editPicInfo, &QWidget::close);

    editLayout->addRow(tr("Путь"), nameEdit);
    editLayout->addRow(browseButton);
    editLayout->addRow(tr("Комментарий"), commentEdit);
    editLayout->addRow(tr("Тэги (через запятую)"), tagEdit);
    editLayout->addRow(editButton);
    editLayout->addRow(cancelButton);

    editPicInfo->setLayout(editLayout);
    editPicInfo->setWindowTitle("Редактировать изображение");
    editPicInfo->setFixedSize(400, 380);
    editPicInfo->show();
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
  const DataWrapper* child = new DataWrapper;
  if (!addSemester)
    child = this->itemData();
  //const DataWrapper* child = current;
  QString child_data;
  QString child_comment;
  QString child_tags;
  QString addWhat = getSubcatName(child->type);

  addWindow = new QWidget;
  addLayout = new QFormLayout;

  if (child->type != PARAGRAPH){
    nameAdd= new QLineEdit;
    nameAdd->setText(child_data);
    commentAdd = new QPlainTextEdit;
    commentAdd->appendPlainText(child_comment);
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
  const DataWrapper* parent = new DataWrapper;
  if (!addSemester)
    parent = this->itemData();
  else
    parent = model->getRoot();

  QModelIndex parent_ind;
  if (!addSemester)
    parent_ind = filteredModel->mapToSource(treeView->selectionModel()->currentIndex());
  //parent = model->dataForIndex(parent_ind);
  int row_count = parent->count;
  //int ins_row = parent_ind.row() + count;

  if (!model->insertRows(row_count, 1,parent_ind))
    return;

  model->fetchAll(parent_ind);
  QModelIndex child = model->index(row_count, 0, parent_ind);
  HData add_data = {(h_type)(1 + (int)parent->type), nameAdd->text(), commentAdd->toPlainText()};
  QVariant add_data_qvariant = QVariant::fromValue(add_data);

  if(!model->setData(child, add_data_qvariant, Qt::EditRole))
    return;
  else{
    qDebug() << "Data was Set!!!!!";
    QMessageBox::information(treeView, "OK", "Успешно!");
    addWindow->close();
    treeView->update(parent_ind);
    }

  updateActions();
  //treeView->update();
}


void MainWindow::addPictures()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  const DataWrapper* parent = this->itemData();

  QModelIndex parent_ind = filteredModel->mapToSource(treeView->selectionModel()->currentIndex());
  parent = model->dataForIndex(parent_ind);
  int row_count = parent->count;
  int how_much = picturePaths.size();

  if (!model->insertRows(row_count, how_much, parent_ind))
    return;

  model->fetchAll(parent_ind);
  for (int i = 0; i < how_much; ++i)
    {
      QModelIndex child = model->index(i + row_count, 0, parent_ind);
      IData add_data = {picturePaths.at(i), pictureComments.at(i)->toPlainText(), pictureTags.at(i)->toPlainText().split(',')};
      QVariant add_data_qvariant = QVariant::fromValue(add_data);
      if(!model->setData(child, add_data_qvariant, Qt::EditRole))
        return;
    }

  qDebug() << "Data was Set!!!!!";
  QApplication::restoreOverrideCursor();
  QMessageBox::information(treeView, "OK", "Успешно!");
  editPicScrollAlrea->close();
  treeView->update(parent_ind);

  updateActions();
}

void MainWindow::edit()
{
  QModelIndex child = filteredModel->mapToSource(treeView->selectionModel()->currentIndex());
  DataWrapper* child_data_wrapper = model->dataForIndex(child);
  HData* child_data = static_cast<HData*>(child_data_wrapper->data);
  if (child_data->name == nameEdit->text() && child_data->comment == commentEdit->toPlainText())
    return;
  HData add_data = {(h_type)(1 + (int)child_data_wrapper->type), nameEdit->text(), commentEdit->toPlainText()};
  QVariant add_data_qvariant = QVariant::fromValue(add_data);

  if(!model->setData(child, add_data_qvariant, Qt::EditRole))
    return;
  else{
    qDebug() << "Data was Set!!!!!";
    treeView->update(child);
    QMessageBox::information(treeView, "OK", "Успешно!");
    editWindow->close();
    catComLabel->setText("<b><Комментарий:/b> " + commentEdit->toPlainText());
    }
}

void MainWindow::editInfo()
{
  QModelIndex child_index = imagesView->selectionModel()->currentIndex();
  DataWrapper* child = static_cast<DataWrapper*>(child_index.internalPointer());
  IData* child_data = static_cast<IData*>(child->data);
  QString path =  child_data->path;
  QString comm = child_data->comment;
  QString tags = child_data->tags.join(",");
  if (path == nameEdit->text() && comm == commentEdit->toPlainText() && tags == tagEdit->toPlainText())
      return;

  IData add_data = {nameEdit->text(), commentEdit->toPlainText(), tagEdit->toPlainText().split(",")};
  QVariant add_data_qvariant = QVariant::fromValue(add_data);

  if(!model->setData(child_index, add_data_qvariant, Qt::EditRole))
    return;
  else{
    qDebug() << "Data was Set!!!!!";
    treeView->update(child_index);
    QMessageBox::information(treeView, "OK", "Успешно!");
    editPicInfo->close();
    imgComLabel->setText(COMMENT_BOLD + commentEdit->toPlainText());
    imgTagLabel->setText(TAGS_BOLD + tagEdit->toPlainText());
    }
}

void MainWindow::browsePic()
{
    nameEdit->setText(QFileDialog::getOpenFileName(0, "Выбор скана лекций", "", "*.jpg *.png *.bmp", 0, 0));
}

void MainWindow::remove()
{
    QModelIndex child = filteredModel->mapToSource(treeView->selectionModel()->currentIndex());
    model->fetchReallyAll(child);
    QModelIndex parent_ind = model->parent(child);
    DataWrapper* child_data_wrapper = model->dataForIndex(child);
    if(!model->removeRows(child_data_wrapper->number, 1, parent_ind))
        return;
    else {
        QMessageBox::information(treeView, "OK", "Успешно!");
        deleteWindow->close();
        catComLabel->setText(COMMENT_BOLD + DEFAULT_COMM_CAT_ITALIC);
     }
}

void MainWindow::removePictures()
{
    DataWrapper* parent = static_cast<DataWrapper *>(currentParagraphIndex.internalPointer());
    for (int i = 0; i < parent->count; )
      {
        DataWrapper* child = parent->children.at(i);
        if (!child->isChecked)
        {
            ++i;
            continue;
        }
        if (!model->removeRows(child->number, 1, currentParagraphIndex))
            return;
        selectedImages.removeAt(i);
        --selectedImagesCount;
      }
    QMessageBox::information(treeView, "OK", "Успешно!");
    imgComLabel->setText(COMMENT_BOLD + DEFAULT_COMM_CAT_ITALIC);
    imgTagLabel->setText(TAGS_BOLD + DEFAULT_COMM_TAGS_IMG_ITALIC);
    updateActions();
    model->fetchAll(currentParagraphIndex);
}

void MainWindow::updateActions()
{
    /*bool has_selection = !treeView->selectionModel()->selection().isEmpty();
    actionDelete->setEnabled(has_selection);

    bool has_current = treeView->selectionModel()->currentIndex().isValid();
    actionAdd->setEnabled(has_current);
    actionEdit->setEnabled(has_current);

    if (has_current)
        treeView->closePersistentEditor(treeView->selectionModel()->currentIndex());*/

    addSemester = false;
}

void MainWindow::addLectures()
{
  addWindow->close();
  picturePaths = QFileDialog::getOpenFileNames(0, "Выбор сканов лекций", "", "*.jpg *.png *.bmp", 0, 0);
  pictureGrid = new QGridLayout;

  size_t num_pics = picturePaths.size();
  if (num_pics == 0)
    return;
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
      QPixmap scaled = pic.scaled(QSize(200, 400), Qt::KeepAspectRatio);
      MyLabel *picLabel = new MyLabel(*it);
      picLabel->setPixmap(scaled);
      picLabel->setCursor(Qt::PointingHandCursor);
      picLayout->addRow(picLabel);
      picLabels.push_back(picLabel);
      picLayouts.push_back(picLayout);
      pictureGrid->addLayout(picLayout, rowCount, colCount++);

      QTextEdit* comLine = new QTextEdit;
      comLine->setFixedHeight(100);
      comLine->setFixedWidth(300);
      comLine->setPlaceholderText("Комментарий");
      comLine->setTabChangesFocus(true);
      comLayout->addRow(comLine);
      comLayouts.push_back(comLayout);
      pictureGrid->addLayout(comLayout, rowCount, colCount++);
      pictureComments.push_back(comLine);

      QTextEdit* tagLine = new QTextEdit;
      tagLine->setFixedHeight(100);
      tagLine->setFixedWidth(300);
      tagLine->setPlaceholderText("Тэги (через запятую)");
      tagLine->setTabChangesFocus(true);
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

      pictureGrid->setRowMinimumHeight(rowCount, 0);

      ++rowCount;
    }

  editPicturesWin = new QWidget;
  editPicScrollAlrea = new QScrollArea;

  QPushButton* addButton = new QPushButton("Добавить");
  connect(addButton, &QPushButton::clicked, this, &MainWindow::addPictures);
  addButton->setMaximumWidth(150);
  QFormLayout* addButLayout = new QFormLayout;
  addButLayout->addRow(addButton);
  pictureGrid->addLayout(addButLayout, rowCount, 0);

  QPushButton* cancelButton = new QPushButton("Отмена");
  connect(cancelButton, &QPushButton::clicked, editPicScrollAlrea, &QScrollArea::close);
  cancelButton->setMaximumWidth(150);
  QFormLayout* cancButLayout = new QFormLayout;
  cancButLayout->addRow(cancelButton);
  pictureGrid->addLayout(cancButLayout, rowCount, 1);

  editPicturesWin->setLayout(pictureGrid);
  editPicturesWin->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  editPicScrollAlrea->setWidget(editPicturesWin);
  editPicScrollAlrea->setWindowTitle("Добавление сканов");
  editPicScrollAlrea->resize(editPicturesWin->width(), editPicScrollAlrea->height());

  editPicScrollAlrea->show();
}

void MainWindow::removePicFromSelection()
{
  for (int i = 0; i < delButtons.size(); ++i)
    if (sender() == delButtons[i]){
        size_t pic_height = picLabels.at(i)->height();

        picturePaths.removeAt(i);

        delete pictureComments.at(i);
        pictureComments.removeAt(i);

        delete pictureTags.at(i);
        pictureTags.removeAt(i);

        delete delButtons.at(i);
        delButtons.removeAt(i);

        delete picLabels.at(i);
        picLabels.removeAt(i);

        picLayouts.removeAt(i);
        comLayouts.removeAt(i);
        tagLayouts.removeAt(i);
        butLayouts.removeAt(i);

        if (pictureComments.size() == 0){
          editPicScrollAlrea->close();
          break;
         }

        pictureGrid->update();

        editPicturesWin->resize(editPicturesWin->width(), editPicturesWin->height() - pic_height);

        break;
      }
}

MyLabel::MyLabel(const QString p)
{
    path = p;
}

void MyLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        ImageViewer* whats_the_picture = new ImageViewer(-1, nullptr, path);
        whats_the_picture->show();
    }
}
