#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_splitter = new QSplitter(Qt::Horizontal, this);
    m_centralWidget = new QWidget(this);
    m_controlsWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    m_mainLayout = new QHBoxLayout(m_centralWidget);
    m_controlsLayout = new QVBoxLayout(m_controlsWidget);

    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene, this);

    m_draw = new Draw(m_scene);

    createControls();

    m_splitter->addWidget(m_view);
    m_splitter->addWidget(m_controlsWidget);
    m_splitter->setStretchFactor(0, 3);
    m_splitter->setStretchFactor(1, 1);

    m_mainLayout->addWidget(m_splitter);

    setWindowTitle("Transformations");
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete m_mainLayout;
    delete m_draw;
    delete m_centralWidget;
}

void MainWindow::createControls()
{
    QLabel* scaleLabel = new QLabel("Pixels per cm:", this);
    m_scaleSpinBox = new QDoubleSpinBox(this);
    m_scaleSpinBox->setRange(10.0, 200.0);
    m_scaleSpinBox->setValue(37.795275591);
    m_scaleSpinBox->setSingleStep(1.0);
    connect(m_scaleSpinBox,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        m_draw, &Draw::setPixelsPerCm);

    QLabel* sizeLabel = new QLabel("Grid size:", this);
    m_gridSizeSpinBox = new QSpinBox(this);
    m_gridSizeSpinBox->setRange(12, 50);
    m_gridSizeSpinBox->setValue(15);
    connect(m_gridSizeSpinBox,
         QOverload<int>::of(&QSpinBox::valueChanged),
         m_draw, &Draw::setGridSize);

    m_noneRadioButton = new QRadioButton("None");
    m_shiftRadioButton = new QRadioButton("Shift");
    m_rotateRadioButton = new QRadioButton("Rotate");
    m_affineRadioButton = new QRadioButton("Affine");
    m_projectiveRadioButton = new QRadioButton("Projective");

    m_transformationGroup = new QButtonGroup(this);
    m_transformationGroup->addButton(m_noneRadioButton, 0);
    m_transformationGroup->addButton(m_shiftRadioButton, 1);
    m_transformationGroup->addButton(m_rotateRadioButton, 2);
    m_transformationGroup->addButton(m_affineRadioButton, 3);
    m_transformationGroup->addButton(m_projectiveRadioButton, 4);
    connect(m_transformationGroup, SIGNAL(buttonClicked(int)), this, SLOT(updateTransformationType(int)));

    m_resetButton = new QPushButton("Reset", this);
    connect(m_resetButton, &QPushButton::clicked, m_draw, &Draw::resetScene);

    QGroupBox *rectangleGroupBox = new QGroupBox("Transformations", m_controlsWidget);
    QVBoxLayout *rectangleLayout = new QVBoxLayout(rectangleGroupBox);
    rectangleLayout->setContentsMargins(10, 10, 10, 10);
    rectangleLayout->setSpacing(10);
    rectangleLayout->setAlignment(Qt::AlignTop);

    m_transformationStack = new QStackedWidget(this);
    m_affineControls = createAffineControls();
    m_projectiveControls = createProjectiveControls();
    m_shiftControls = createShiftControls();
    m_rotateControls = createRotateControls();
    m_transformationStack->addWidget(new QWidget());
    m_transformationStack->addWidget(m_shiftControls);
    m_transformationStack->addWidget(m_rotateControls);
    m_transformationStack->addWidget(m_affineControls);
    m_transformationStack->addWidget(m_projectiveControls);

    rectangleLayout->addWidget(m_transformationStack, 0, Qt::AlignCenter);
    rectangleGroupBox->setMinimumSize(300, 200);
    rectangleGroupBox->setMaximumSize(600, 600);
    
    m_controlsLayout->addWidget(scaleLabel);
    m_controlsLayout->addWidget(m_scaleSpinBox);
    m_controlsLayout->addSpacing(20);
    m_controlsLayout->addWidget(sizeLabel);
    m_controlsLayout->addWidget(m_gridSizeSpinBox);
    m_controlsLayout->addSpacing(20);
    m_controlsLayout->addWidget(m_noneRadioButton);
    m_controlsLayout->addWidget(m_shiftRadioButton);
    m_controlsLayout->addWidget(m_rotateRadioButton);
    m_controlsLayout->addWidget(m_affineRadioButton);
    m_controlsLayout->addWidget(m_projectiveRadioButton);
    m_controlsLayout->addSpacing(20);
    m_controlsLayout->addWidget(rectangleGroupBox);
    m_controlsLayout->addWidget(m_resetButton);
    m_controlsLayout->addStretch();
}

void MainWindow::updateScale(double value)
{
    m_draw->setPixelsPerCm(value);
}

void MainWindow::updateGridSize(int value)
{
    m_draw->setGridSize(value);
}

QWidget* MainWindow::createProjectiveControls()
{
    auto *projectiveWidget = new QWidget(this);
    m_transformationLayout = new QVBoxLayout(projectiveWidget);

    auto* projectiveLabel = new QLabel("Projective Transformation:", this);

    auto* matrixLayout = new QGridLayout();

    auto* XxProjectiveLabel = new QLabel("Xx:", this);
    XxProjective = new QDoubleSpinBox(this);
    XxProjective->setRange(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
    XxProjective->setValue(1.0);

    auto* XyProjectiveLabel = new QLabel("Xy:", this);
    XyProjective = new QDoubleSpinBox(this);
    XyProjective->setRange(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
    XyProjective->setValue(0.0);

    auto* XwProjectiveLabel = new QLabel("Xw:", this);
    XwProjective = new QDoubleSpinBox(this);
    XwProjective->setRange(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
    XwProjective->setValue(0.0);

    auto* YxProjectiveLabel = new QLabel("Yx:", this);
    YxProjective = new QDoubleSpinBox(this);
    YxProjective->setRange(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
    YxProjective->setValue(0.0);

    auto* YyProjectiveLabel = new QLabel("Yy:", this);
    YyProjective = new QDoubleSpinBox(this);
    YyProjective->setRange(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
    YyProjective->setValue(1.0);

    auto* YwProjectiveLabel = new QLabel("Yw:", this);
    YwProjective = new QDoubleSpinBox(this);
    YwProjective->setRange(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
    YwProjective->setValue(0.0);

    auto* OxProjectiveLabel = new QLabel("Ox:", this);
    OxProjective = new QDoubleSpinBox(this);
    OxProjective->setRange(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
    OxProjective->setValue(0.0);

    auto* OyProjectiveLabel = new QLabel("Oy:", this);
    OyProjective = new QDoubleSpinBox(this);
    OyProjective->setRange(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
    OyProjective->setValue(0.0);

    auto* OwProjectiveLabel = new QLabel("Ow:", this);
    OwProjective = new QDoubleSpinBox(this);
    OwProjective->setRange(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
    OwProjective->setValue(1.0);

    matrixLayout->addWidget(XxProjectiveLabel, 0, 0);
    matrixLayout->addWidget(XxProjective, 0, 1);
    matrixLayout->addWidget(XyProjectiveLabel, 0, 2);
    matrixLayout->addWidget(XyProjective, 0, 3);
    matrixLayout->addWidget(XwProjectiveLabel, 0, 4);
    matrixLayout->addWidget(XwProjective, 0, 5);

    matrixLayout->addWidget(YxProjectiveLabel, 1, 0);
    matrixLayout->addWidget(YxProjective, 1, 1);
    matrixLayout->addWidget(YyProjectiveLabel, 1, 2);
    matrixLayout->addWidget(YyProjective, 1, 3);
    matrixLayout->addWidget(YwProjectiveLabel, 1, 4);
    matrixLayout->addWidget(YwProjective, 1, 5);

    matrixLayout->addWidget(OxProjectiveLabel, 2, 0);
    matrixLayout->addWidget(OxProjective, 2, 1);
    matrixLayout->addWidget(OyProjectiveLabel, 2, 2);
    matrixLayout->addWidget(OyProjective, 2, 3);
    matrixLayout->addWidget(OwProjectiveLabel, 2, 4);
    matrixLayout->addWidget(OwProjective, 2, 5);

    m_projectiveButton = new QPushButton("Apply", this);
    connect(m_projectiveButton, &QPushButton::clicked, [this]() {
        double Xx = XxProjective->value();
        double Xy = XyProjective->value();
        double Xw = XwProjective->value();
        double Yx = YxProjective->value();
        double Yy = YyProjective->value();
        double Yw = YwProjective->value();
        double Ox = OxProjective->value();
        double Oy = OyProjective->value();
        double Ow = OwProjective->value();

        m_draw->applyProjectiveTransformation(Xx, Xy, Xw, Yx, Yy, Yw, Ox, Oy, Ow);
    });

    m_transformationLayout->addWidget(projectiveLabel);
    m_transformationLayout->addLayout(matrixLayout);
    m_transformationLayout->addWidget(m_projectiveButton);

    return projectiveWidget;
}

QWidget* MainWindow::createAffineControls()
{
    auto* affineWidget = new QWidget(this);
    m_transformationLayout = new QVBoxLayout(affineWidget);

    auto* affineLabel = new QLabel("Affine Transformation:", this);

    auto* matrixLayout = new QGridLayout();

    auto* XxAffineLabel = new QLabel("Xx:", this);
    XxAffine = new QDoubleSpinBox(this);
    XxAffine->setRange(-10.0, 10.0);
    XxAffine->setValue(1.0);

    auto* XyAffineLabel = new QLabel("Xy:", this);
    XyAffine = new QDoubleSpinBox(this);
    XyAffine->setRange(-10.0, 10.0);
    XyAffine->setValue(0.0);

    auto* YxAffineLabel = new QLabel("Yx:", this);
    YxAffine = new QDoubleSpinBox(this);
    YxAffine->setRange(-10.0, 10.0);
    YxAffine->setValue(0.0);

    auto* YyAffineLabel = new QLabel("Yy:", this);
    YyAffine = new QDoubleSpinBox(this);
    YyAffine->setRange(-10.0, 10.0);
    YyAffine->setValue(1.0);

    auto* OxAffineLabel = new QLabel("Ox:", this);
    OxAffine = new QDoubleSpinBox(this);
    OxAffine->setRange(-10.0, 10.0);
    OxAffine->setValue(0.0);

    auto* OyAffineLabel = new QLabel("Oy:", this);
    OyAffine = new QDoubleSpinBox(this);
    OyAffine->setRange(-10.0, 10.0);
    OyAffine->setValue(0.0);

    matrixLayout->addWidget(XxAffineLabel, 0, 0);
    matrixLayout->addWidget(XxAffine, 0, 1);
    matrixLayout->addWidget(XyAffineLabel, 0, 2);
    matrixLayout->addWidget(XyAffine, 0, 3);

    matrixLayout->addWidget(YxAffineLabel, 1, 0);
    matrixLayout->addWidget(YxAffine, 1, 1);
    matrixLayout->addWidget(YyAffineLabel, 1, 2);
    matrixLayout->addWidget(YyAffine, 1, 3);

    matrixLayout->addWidget(OxAffineLabel, 2, 0);
    matrixLayout->addWidget(OxAffine, 2, 1);
    matrixLayout->addWidget(OyAffineLabel, 2, 2);
    matrixLayout->addWidget(OyAffine, 2, 3);

    m_affineButton = new QPushButton("Apply", this);
    connect(m_affineButton, &QPushButton::clicked, [this]() {
        double Xx = XxAffine->value();
        double Xy = XyAffine->value();
        double Yx = YxAffine->value();
        double Yy = YyAffine->value();
        double Ox = OxAffine->value();
        double Oy = OyAffine->value();

        m_draw->applyAffineTransformation(Xx, Xy, Yx, Yy, Ox, Oy);
    });

    m_transformationLayout->addWidget(affineLabel);
    m_transformationLayout->addLayout(matrixLayout);
    m_transformationLayout->addWidget(m_affineButton);

    return affineWidget;
}

QWidget* MainWindow::createShiftControls() {
    auto* shiftWidget = new QWidget(this);

    auto* shiftLayout = new QVBoxLayout(shiftWidget);

    shiftLayout->setSpacing(5);
    shiftLayout->setContentsMargins(5, 5, 5, 5);

    auto* shiftControlsLayout = new QHBoxLayout();
    shiftControlsLayout->setSpacing(5);

    QLabel* shiftLabel = new QLabel("Shifting:", this);

    QLabel* shiftXLabel = new QLabel("Shift X:", this);
    m_shiftXSpinBox = new QDoubleSpinBox(this);
    m_shiftXSpinBox->setValue(0.0);

    QLabel* shiftYLabel = new QLabel("Shift Y:", this);
    m_shiftYSpinBox = new QDoubleSpinBox(this);
    m_shiftYSpinBox->setValue(0.0);

    shiftControlsLayout->addWidget(shiftXLabel);
    shiftControlsLayout->addWidget(m_shiftXSpinBox);
    shiftControlsLayout->addWidget(shiftYLabel);
    shiftControlsLayout->addWidget(m_shiftYSpinBox);

    m_shiftButton = new QPushButton("Apply", this);
    connect(m_shiftButton, &QPushButton::clicked, [this]() {
        double shiftX = m_shiftXSpinBox->value();
        double shiftY = m_shiftYSpinBox->value();
        m_draw->applyShiftToFigure(shiftX, shiftY);
    });

    shiftLayout->addWidget(shiftLabel);
    shiftLayout->addLayout(shiftControlsLayout);
    shiftLayout->addWidget(m_shiftButton);

    return shiftWidget;
}

QWidget *MainWindow::createRotateControls() {
    auto* rotateWidget = new QWidget(this);

    auto* rotateLayout = new QVBoxLayout(rotateWidget);

    rotateLayout->setSpacing(5);
    rotateLayout->setContentsMargins(5, 5, 5, 5);

    auto* rotateControlsLayout = new QHBoxLayout();
    rotateControlsLayout->setSpacing(5);

    QLabel *rotateLabel = new QLabel("Rotation:", this);

    QLabel* xLabel = new QLabel("X:", this);
    m_xRotateSpinBox = new QDoubleSpinBox(this);
    m_xRotateSpinBox->setValue(0.0);

    QLabel* yLabel = new QLabel("Y:", this);
    m_yRotateSpinBox = new QDoubleSpinBox(this);
    m_yRotateSpinBox->setValue(0.0);

    QLabel* angleLable = new QLabel("angle:", this);
    m_angleRotateSpinBox = new QDoubleSpinBox(this);
    m_angleRotateSpinBox->setValue(0.0);
    m_angleRotateSpinBox->setRange(-360.0, 360.0);

    rotateControlsLayout->addWidget(xLabel);
    rotateControlsLayout->addWidget(m_xRotateSpinBox);
    rotateControlsLayout->addWidget(yLabel);
    rotateControlsLayout->addWidget(m_yRotateSpinBox);
    rotateControlsLayout->addWidget(angleLable);
    rotateControlsLayout->addWidget(m_angleRotateSpinBox);

    m_rotateButton = new QPushButton("Apply", this);
    connect(m_rotateButton, &QPushButton::clicked, [this]() {
        double x = m_xRotateSpinBox->value();
        double y = m_yRotateSpinBox->value();
        double angle = m_angleRotateSpinBox->value();

        m_draw->applyRotateToFigure(x, y, angle);
    });

    rotateLayout->addWidget(rotateLabel);
    rotateLayout->addLayout(rotateControlsLayout);
    rotateLayout->addWidget(m_rotateButton);

    return rotateWidget;
}

void MainWindow::updateTransformationType(const int id) const {
    m_transformationStack->setCurrentIndex(id);
}


