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

    // Create the scene and view
    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene, this);

    // Create the coordinate grid
    m_draw = new Draw(m_scene);

    // Create and set up controls
    createControls();

    m_splitter->addWidget(m_view);
    m_splitter->addWidget(m_controlsWidget);
    m_splitter->setStretchFactor(0, 3);
    m_splitter->setStretchFactor(1, 1);

    // Add widgets to layouts
    m_mainLayout->addWidget(m_splitter);

    // Set initial window properties
    setWindowTitle("Transformations");
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete m_draw;
    delete m_centralWidget;
}

void MainWindow::createControls()
{
   // Scale control
    QLabel* scaleLabel = new QLabel("Pixels per cm:", this);
    m_scaleSpinBox = new QDoubleSpinBox(this);
    m_scaleSpinBox->setRange(10.0, 200.0);
    m_scaleSpinBox->setValue(37.795275591);
    m_scaleSpinBox->setSingleStep(1.0);
    connect(m_scaleSpinBox,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        m_draw, &Draw::setPixelsPerCm);
    //connect(m_draw, &Draw::pixelsPerCmChanged, m_scaleSpinBox, &QDoubleSpinBox::setValue);


    // Grid size control
    QLabel* sizeLabel = new QLabel("Grid size:", this);
    m_gridSizeSpinBox = new QSpinBox(this);
    m_gridSizeSpinBox->setRange(1, 50);
    m_gridSizeSpinBox->setValue(10);
    connect(m_gridSizeSpinBox,
         QOverload<int>::of(&QSpinBox::valueChanged),
         m_draw, &Draw::setGridSize);

    // Zoom buttons
  //  m_zoomInButton = new QPushButton("+", this);
  //  m_zoomOutButton = new QPushButton("-", this);
   // connect(m_zoomInButton, &QPushButton::clicked, m_draw, &Draw::zoomIn);
    //connect(m_zoomOutButton, &QPushButton::clicked, m_draw, &Draw::zoomOut);

    m_noneRadioButton = new QRadioButton("None");
    m_affineRadioButton = new QRadioButton("Affine");
    m_projectiveRadioButton = new QRadioButton("Projective");

    m_transformationGroup = new QButtonGroup(this);
    m_transformationGroup->addButton(m_noneRadioButton, 0);
    m_transformationGroup->addButton(m_affineRadioButton, 1);
    m_transformationGroup->addButton(m_projectiveRadioButton, 2);
    connect(m_transformationGroup, SIGNAL(buttonClicked(int)), this, SLOT(updateTransformationType(int)));

    m_resetButton = new QPushButton("Reset", this);
    connect(m_resetButton, &QPushButton::clicked, this, &MainWindow::resetTransformation);

    // Add controls to app
    m_transformationStack = new QStackedWidget(this);
    m_affineControls = createAffineControls();
    m_projectiveControls = createProjectiveControls();
    m_transformationStack->addWidget(new QWidget()); // Empty widget for "None"
    m_transformationStack->addWidget(m_affineControls);
    m_transformationStack->addWidget(m_projectiveControls);
    m_controlsLayout->addWidget(scaleLabel);
    m_controlsLayout->addWidget(m_scaleSpinBox);
    m_controlsLayout->addSpacing(20);
    m_controlsLayout->addWidget(sizeLabel);
    m_controlsLayout->addWidget(m_gridSizeSpinBox);
    m_controlsLayout->addSpacing(20);
  //  m_controlsLayout->addWidget(m_zoomInButton);
   // m_controlsLayout->addWidget(m_zoomOutButton);
    m_controlsLayout->addSpacing(20);
    m_controlsLayout->addWidget(m_noneRadioButton);
    m_controlsLayout->addWidget(m_affineRadioButton);
    m_controlsLayout->addWidget(m_projectiveRadioButton);
    m_controlsLayout->addWidget(m_transformationStack);
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

    // Projective transformation controls
    auto* projectiveLabel = new QLabel("Projective Transformation:", this);
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

        // Call the transformation function with the retrieved values
        m_draw->applyProjectiveTransformation(Xx, Xy, Xw, Yx, Yy, Yw, Ox, Oy, Ow);
    });

    m_transformationLayout->addWidget(projectiveLabel);
    m_transformationLayout->addWidget(XxProjectiveLabel);
    m_transformationLayout->addWidget(XxProjective);
    m_transformationLayout->addWidget(XyProjectiveLabel);
    m_transformationLayout->addWidget(XyProjective);
    m_transformationLayout->addWidget(XwProjectiveLabel);
    m_transformationLayout->addWidget(XwProjective);
    m_transformationLayout->addWidget(YxProjectiveLabel);
    m_transformationLayout->addWidget(YxProjective);
    m_transformationLayout->addWidget(YyProjectiveLabel);
    m_transformationLayout->addWidget(YyProjective);
    m_transformationLayout->addWidget(YwProjectiveLabel);
    m_transformationLayout->addWidget(YwProjective);
    m_transformationLayout->addWidget(OxProjectiveLabel);
    m_transformationLayout->addWidget(OxProjective);
    m_transformationLayout->addWidget(OyProjectiveLabel);
    m_transformationLayout->addWidget(OyProjective);
    m_transformationLayout->addWidget(OwProjectiveLabel);
    m_transformationLayout->addWidget(OwProjective);
    m_transformationLayout->addWidget(m_projectiveButton);

    return projectiveWidget;
}

QWidget* MainWindow::createAffineControls()
{
    auto* affineWidget = new QWidget(this);
    m_transformationLayout = new QVBoxLayout(affineWidget);

    // Affine transformation controls
    auto* affineLabel = new QLabel("Affine Transformation:", this);
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
    m_affineButton = new QPushButton("Apply", this);
    connect(m_affineButton, &QPushButton::clicked, [this]() {
        double Xx = XxAffine->value();
        double Xy = XyAffine->value();
        double Yx = YxAffine->value();
        double Yy = YyAffine->value();
        double Ox = OxAffine->value();
        double Oy = OyAffine->value();

        // Call the transformation function with the retrieved values
        m_draw->applyAffineTransformation(Xx, Xy, Yx, Yy, Ox, Oy);
    });

    m_transformationLayout->addWidget(affineLabel);
    m_transformationLayout->addWidget(XxAffineLabel);
    m_transformationLayout->addWidget(XxAffine);
    m_transformationLayout->addWidget(XyAffineLabel);
    m_transformationLayout->addWidget(XyAffine);
    m_transformationLayout->addWidget(YxAffineLabel);
    m_transformationLayout->addWidget(YxAffine);
    m_transformationLayout->addWidget(YyAffineLabel);
    m_transformationLayout->addWidget(YyAffine);
    m_transformationLayout->addWidget(OxAffineLabel);
    m_transformationLayout->addWidget(OxAffine);
    m_transformationLayout->addWidget(OyAffineLabel);
    m_transformationLayout->addWidget(OyAffine);
    m_transformationLayout->addWidget(m_affineButton);

    return affineWidget;
}

void MainWindow::updateTransformationType(const int id) const {
    m_transformationStack->setCurrentIndex(id);
}

void MainWindow::resetTransformation()
{
    return;
}

