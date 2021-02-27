#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(AbstractEquationSystem* system, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setFixedSize(675, 440);
    ui->setupUi(this);

    m_system = system;
    m_solution = nullptr;

    ui->eq_1->setText(" = " + m_system->f1Str());
    ui->eq_2->setText(" = " + m_system->f2Str());
    ui->eq_3->setText(" = " + m_system->f3Str());

    connect(ui->action_start, &QAction::triggered, this, &MainWindow::startOver);
    connect(ui->solve, &QPushButton::clicked, this, &MainWindow::solve);
    connect(ui->action_help, &QAction::triggered, this, &MainWindow::showHelpDialog);
    connect(ui->action_about, &QAction::triggered, this, &MainWindow::showAboutDialog);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (m_solution != nullptr)
        delete m_solution;
}

void MainWindow::showAboutDialog()
{
    AboutDialog dialog;
    dialog.exec();
}

void MainWindow::showHelpDialog()
{
    HelpDialog dialog;
    dialog.exec();
}

void MainWindow::startOver()
{

}

void MainWindow::solve()
{
    double a = ui->segment_begin->value();
    double b = ui->segment_end->value();
    double n = ui->segment_steps->value();
    std::array<double, 3> init_conditions = {
        ui->init_x->value(),
        ui->inix_y->value(),
        ui->init_z->value()
    };
    RKMethodSolver solver(m_system);
    std::vector<std::array<double, 4>> solutions;
    try
    {
        solutions = solver.solve(a, b, n, init_conditions);
    }
    catch (std::exception& e)
    {
        QMessageBox::warning(this, "Computation error", e.what());
    }
    if (m_solution != nullptr)
        delete m_solution;
    m_solution = new SolutionTableModel(solutions, ui->solution);
    ui->solution->setModel(m_solution);
}
