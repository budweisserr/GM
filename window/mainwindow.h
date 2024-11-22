#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void resetTransformation();

private:
    void createControls();
    QWidget* createAffineControls();
    QWidget* createProjectiveControls();

    QGraphicsView *m_view;
    QGraphicsScene *m_scene;

    Draw* m_draw;
    QWidget* m_centralWidget;
    QWidget *m_controlsWidget;
    QWidget *m_sceneWidget;
    QHBoxLayout *m_mainLayout;
    QVBoxLayout *m_controlsLayout;
    QVBoxLayout *m_transformationLayout;

    QDoubleSpinBox* m_scaleSpinBox;
    QSpinBox* m_gridSizeSpinBox;
    QPushButton* m_zoomInButton;
    QPushButton* m_zoomOutButton;
    QButtonGroup* m_transformationGroup;
    QPushButton* m_resetButton;

    QDoubleSpinBox* XxAffine;
    QDoubleSpinBox* XyAffine;
    QDoubleSpinBox* YxAffine;
    QDoubleSpinBox* YyAffine;
    QDoubleSpinBox* OxAffine;
    QDoubleSpinBox* OyAffine;
    QPushButton* m_affineButton;

    QDoubleSpinBox *XxProjective;
    QDoubleSpinBox *XyProjective;
    QDoubleSpinBox *XwProjective;
    QDoubleSpinBox *YxProjective;
    QDoubleSpinBox *YyProjective;
    QDoubleSpinBox *YwProjective;
    QDoubleSpinBox *OxProjective;
    QDoubleSpinBox *OyProjective;
    QDoubleSpinBox *OwProjective;
    QPushButton *m_projectiveButton;


    QRadioButton* m_noneRadioButton;
    QRadioButton* m_affineRadioButton;
    QRadioButton* m_projectiveRadioButton;
    QStackedWidget* m_transformationStack;
    QWidget* m_affineControls;
    QWidget* m_projectiveControls;

    QSplitter *m_splitter;

};

#endif // MAINWINDOW_H