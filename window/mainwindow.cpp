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
    resize(1920, 1080);
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
    m_scaleSpinBox->setRange(0, 200.0);
    m_scaleSpinBox->setValue(37.795275591);
    m_scaleSpinBox->setSingleStep(1.0);
    connect(m_scaleSpinBox,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        m_draw, &Draw::setPixelsPerCm);
    connect(m_draw, &Draw::pixelsPerCmChanged, m_scaleSpinBox, &QDoubleSpinBox::setValue);

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
    m_animationRadioButton = new QRadioButton("Animation");

    m_transformationGroup = new QButtonGroup(this);
    m_transformationGroup->addButton(m_noneRadioButton, 0);
    m_transformationGroup->addButton(m_shiftRadioButton, 1);
    m_transformationGroup->addButton(m_rotateRadioButton, 2);
    m_transformationGroup->addButton(m_animationRadioButton, 3);
    connect(m_transformationGroup, SIGNAL(buttonClicked(int)), this, SLOT(updateTransformationType(int)));

    m_resetButton = new QPushButton("Reset", this);
    connect(m_resetButton, &QPushButton::clicked, m_draw, &Draw::resetScene);

    QGroupBox *rectangleGroupBox = new QGroupBox("Transformations", m_controlsWidget);
    QVBoxLayout *rectangleLayout = new QVBoxLayout(rectangleGroupBox);
    rectangleLayout->setContentsMargins(10, 10, 10, 10);
    rectangleLayout->setSpacing(10);
    rectangleLayout->setAlignment(Qt::AlignTop);


    m_showPointCheckBox = new QCheckBox("Show Point", this);
    m_showNormalCheckBox = new QCheckBox("Show Normal", this);
    m_showTangentCheckBox = new QCheckBox("Show Tangent", this);

    connect(m_showNormalCheckBox, &QCheckBox::stateChanged, m_draw, &Draw::toggleNormalVisibility);
    connect(m_showTangentCheckBox, &QCheckBox::stateChanged, m_draw, &Draw::toggleTangentVisibility);
    connect (m_showPointCheckBox, &QCheckBox::stateChanged, this, &MainWindow::togglePoint);
    connect(m_showPointCheckBox, &QCheckBox::stateChanged, m_draw, &Draw::togglePointVisibility);

    m_transformationStack = new QStackedWidget(this);
    m_shiftControls = createShiftControls();
    m_rotateControls = createRotateControls();
    m_animationControls = createAnimationControls();
    m_transformationStack->addWidget(new QWidget());
    m_transformationStack->addWidget(m_shiftControls);
    m_transformationStack->addWidget(m_rotateControls);
    m_transformationStack->addWidget(m_animationControls);

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
    m_controlsLayout->addWidget(m_animationRadioButton);
    m_controlsLayout->addSpacing(20);
    m_controlsLayout->addWidget(rectangleGroupBox);
    m_controlsLayout->addWidget(m_resetButton);
    m_controlsLayout->addWidget(m_showPointCheckBox);
    m_controlsLayout->addWidget(m_showNormalCheckBox);
    m_controlsLayout->addWidget(m_showTangentCheckBox);
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
    m_shiftXSpinBox->setRange(-999, std::numeric_limits<double>::max());

    QLabel* shiftYLabel = new QLabel("Shift Y:", this);
    m_shiftYSpinBox = new QDoubleSpinBox(this);
    m_shiftYSpinBox->setValue(0.0);
    m_shiftYSpinBox->setRange(-999, std::numeric_limits<double>::max());

    shiftControlsLayout->addWidget(shiftXLabel);
    shiftControlsLayout->addWidget(m_shiftXSpinBox);
    shiftControlsLayout->addWidget(shiftYLabel);
    shiftControlsLayout->addWidget(m_shiftYSpinBox);

    m_shiftButton = new QPushButton("Apply", this);
    connect(m_shiftButton, &QPushButton::clicked, [this]() {
        double shiftX = m_shiftXSpinBox->value();
        double shiftY = m_shiftYSpinBox->value();
        m_draw->applyShiftToShape(shiftX, shiftY);
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
    m_xRotateSpinBox->setRange(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());

    QLabel* yLabel = new QLabel("Y:", this);
    m_yRotateSpinBox = new QDoubleSpinBox(this);
    m_yRotateSpinBox->setValue(0.0);
    m_yRotateSpinBox->setRange(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());

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

        m_draw->applyRotateToShape(x, y, angle);
    });

    rotateLayout->addWidget(rotateLabel);
    rotateLayout->addLayout(rotateControlsLayout);
    rotateLayout->addWidget(m_rotateButton);

    return rotateWidget;
}

QWidget *MainWindow::createAnimationControls() {
    auto *animationWidget = new QWidget(this);

    auto *animationLayout = new QVBoxLayout(animationWidget);

    animationLayout->setSpacing(5);
    animationLayout->setContentsMargins(5, 5, 5, 5);

    auto *animationControlsLayout = new QVBoxLayout();
    animationControlsLayout->setSpacing(5);

    QLabel *animationLabel = new QLabel("Animation:", this);

    QLabel *animationALabel = new QLabel("a:", this);
    m_animationASpinBox = new QDoubleSpinBox(this);
    m_animationASpinBox->setValue(0.0);
    m_animationASpinBox->setRange(-999, std::numeric_limits<double>::max());

    m_animationAButton = new QPushButton("Apply", this);

    QLabel *animationBLabel = new QLabel("b:", this);
    m_animationBSpinBox = new QDoubleSpinBox(this);
    m_animationBSpinBox->setValue(0.0);
    m_animationBSpinBox->setRange(-999, std::numeric_limits<double>::max());

    m_animationBButton = new QPushButton("Apply", this);

    QLabel *animationCLabel = new QLabel("d:", this);
    m_animationCSpinBox = new QDoubleSpinBox(this);
    m_animationCSpinBox->setValue(0.0);
    m_animationCSpinBox->setRange(-999, std::numeric_limits<double>::max());

    m_animationCButton = new QPushButton("Apply", this);

    m_animationFullButton = new QPushButton("Apply to all", this);

    connect(m_animationAButton, &QPushButton::clicked, [this]() {
        m_draw->animateShape(AnimationType::ANIMATE_A);
    });

    connect(m_animationBButton, &QPushButton::clicked, [this]() {
        m_draw->animateShape(AnimationType::ANIMATE_B);
    });

    connect(m_animationCButton, &QPushButton::clicked, [this]() {
        m_draw->animateShape(AnimationType::ANIMATE_C);
    });

    connect(m_animationFullButton, &QPushButton::clicked, [this]() {
        m_draw->animateShape(AnimationType::ANIMATE_FULL);
    });

    connect(m_draw, &Draw::animationChangedA, m_animationASpinBox, &QDoubleSpinBox::setValue);
    connect(m_draw, &Draw::animationChangedB, m_animationBSpinBox, &QDoubleSpinBox::setValue);
    connect(m_draw, &Draw::animationChangedC, m_animationCSpinBox, &QDoubleSpinBox::setValue);

    animationControlsLayout->addWidget(animationALabel);
    animationControlsLayout->addWidget(m_animationASpinBox);
    animationControlsLayout->addWidget(m_animationAButton);

    animationControlsLayout->addWidget(animationBLabel);
    animationControlsLayout->addWidget(m_animationBSpinBox);
    animationControlsLayout->addWidget(m_animationBButton);

    animationControlsLayout->addWidget(animationCLabel);
    animationControlsLayout->addWidget(m_animationCSpinBox);
    animationControlsLayout->addWidget(m_animationCButton);

    animationLayout->addWidget(animationLabel);
    animationLayout->addLayout(animationControlsLayout);
    animationLayout->addWidget(m_animationFullButton);

    return animationWidget;
}

QWidget* MainWindow::createPointControls() {
    auto* pointWidget = new QWidget(this);

    auto* pointLayout = new QVBoxLayout(pointWidget);
    pointLayout->setSpacing(5);
    pointLayout->setContentsMargins(5, 5, 5, 5);
    connect(m_draw, &Draw::currentPointChanged, this, &MainWindow::updateCurrentPointLabel);

    QLabel* pointLabel = new QLabel("Current Point:", this);
    currentPointLabel = new QLabel("0,0", this);

    QPushButton* prevPointButton = new QPushButton("Previous Point", this);
    QPushButton* nextPointButton = new QPushButton("Next Point", this);

    connect(prevPointButton, &QPushButton::clicked, m_draw, &Draw::showPreviousPoint);

    connect(nextPointButton, &QPushButton::clicked, m_draw,&Draw::showNextPoint);


    pointLayout->addWidget(pointLabel);
    pointLayout->addWidget(currentPointLabel);
    pointLayout->addWidget(prevPointButton);
    pointLayout->addWidget(nextPointButton);

    return pointWidget;
}

void MainWindow::updateTransformationType(const int id) const {
    m_transformationStack->setCurrentIndex(id);
}

void MainWindow::togglePoint(int state) {
    if (state == Qt::Checked) {
        if (!m_pointControlsWidget) {
            m_pointControlsWidget = createPointControls();
        }
        m_controlsLayout->addWidget(m_pointControlsWidget);
        m_pointControlsWidget->show();
        updateCurrentPointLabel(QString("%1, %2").arg(m_draw->getPoints().at(0).x() / m_scaleSpinBox->value()).arg(m_draw->getPoints().at(0).y() / m_scaleSpinBox->value()));
    } else {
        if (m_pointControlsWidget) {
            m_controlsLayout->removeWidget(m_pointControlsWidget);
            m_pointControlsWidget->hide();
        }
    }
}

void MainWindow::updateCurrentPointLabel(const QString& point) {
    currentPointLabel->setText(point);
}


