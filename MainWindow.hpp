#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include "Game.hpp"

namespace coup_gui {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddPlayerClicked();
    void onStartGameClicked();
    void onPerformAction();
    void onActionSelected(const QString &action);

private:
    coup::Player* current;
    void setupInitialUI();
    void setupGameplayUI();
    void refreshPlayerList();
    void updateActions();
    void updateTargets();
    void updateUI();

    // Widgets
    QWidget *central;
    QLineEdit *nameInput;
    QPushButton *addPlayerBtn;
    QPushButton *startGameBtn;
    QListWidget *playerList;
    QLabel *currentPlayerLabel;
    QComboBox *actionBox;
    QComboBox *targetBox;
    QPushButton *performActionBtn;
    QPushButton *performButton;


    // Game logic
    coup::Game *game;
    bool gameStarted;
    static const size_t MAX_PLAYERS = 6;
};

} // namespace coup_gui

