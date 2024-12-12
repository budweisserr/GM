#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QSplitter>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QGraphicsProxyWidget>
#include "drawing.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    private slots:
    void updateScale(double value);
    void updateGridSize(int value);
    void updateTransformationType(int id) const;
    void updateCurrentPointLabel(const QString& point);

private:
    void createControls();
    QWidget* createShiftControls();
    QWidget* createRotateControls();
    QWidget* createAnimationControls();
    QWidget* createPointControls();

    void togglePoint(int state);

    QGraphicsView *m_view = nullptr;
    QGraphicsScene *m_scene = nullptr;

    Draw* m_draw = nullptr;
    QWidget *m_centralWidget = nullptr;
    QWidget *m_controlsWidget = nullptr;
    QWidget *m_sceneWidget = nullptr;
    QHBoxLayout *m_mainLayout = nullptr;
    QVBoxLayout *m_controlsLayout = nullptr;
    QVBoxLayout *m_transformationLayout = nullptr;

    QDoubleSpinBox* m_scaleSpinBox = nullptr;
    QSpinBox* m_gridSizeSpinBox = nullptr;
    QButtonGroup* m_transformationGroup = nullptr;
    QPushButton* m_resetButton = nullptr;

    QDoubleSpinBox *m_shiftXSpinBox = nullptr;
    QDoubleSpinBox *m_shiftYSpinBox = nullptr;
    QPushButton *m_shiftButton = nullptr;

    QDoubleSpinBox *m_xRotateSpinBox = nullptr;
    QDoubleSpinBox *m_yRotateSpinBox = nullptr;
    QDoubleSpinBox *m_angleRotateSpinBox = nullptr;
    QPushButton *m_rotateButton = nullptr;

    QDoubleSpinBox *m_animationASpinBox = nullptr;
    QDoubleSpinBox *m_animationBSpinBox = nullptr;
    QDoubleSpinBox *m_animationCSpinBox = nullptr;
    QPushButton *m_animationFullButton = nullptr;
    QPushButton *m_animationAButton = nullptr;
    QPushButton *m_animationBButton = nullptr;
    QPushButton *m_animationCButton = nullptr;

    QRadioButton* m_noneRadioButton = nullptr;
    QRadioButton* m_shiftRadioButton = nullptr;
    QRadioButton* m_rotateRadioButton = nullptr;
    QRadioButton* m_animationRadioButton = nullptr;
    QStackedWidget* m_transformationStack = nullptr;
    QWidget* m_shiftControls = nullptr;
    QWidget* m_rotateControls = nullptr;
    QWidget* m_animationControls = nullptr;

    QCheckBox *m_showPointCheckBox = nullptr;
    QCheckBox *m_showNormalCheckBox = nullptr;
    QCheckBox *m_showTangentCheckBox = nullptr;
    QWidget *m_pointControlsWidget = nullptr;

    QSplitter *m_splitter = nullptr;
    QLabel* currentPointLabel = nullptr;

};

#endif // MAINWINDOW_H