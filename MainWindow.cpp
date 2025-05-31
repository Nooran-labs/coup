// Updated MainWindow.cpp with safety checks and initialization
#include "MainWindow.hpp"
#include "Player.hpp"
#include <QInputDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

namespace coup_gui {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), game(nullptr), current(nullptr) {
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *layout = new QVBoxLayout(central);
    addPlayerBtn = new QPushButton("Add Player", this);
    startGameBtn = new QPushButton("Start Game", this);
    performButton = new QPushButton("Perform Action", this);
    playerList = new QListWidget(this);
    actionBox = new QComboBox(this);
    targetBox = new QComboBox(this);
    currentPlayerLabel = new QLabel("", this);

    layout->addWidget(addPlayerBtn);
    layout->addWidget(startGameBtn);
    layout->addWidget(playerList);
    layout->addWidget(currentPlayerLabel);
    layout->addWidget(actionBox);
    layout->addWidget(targetBox);
    layout->addWidget(performButton);

    connect(addPlayerBtn, &QPushButton::clicked, this, &MainWindow::onAddPlayerClicked);
    connect(startGameBtn, &QPushButton::clicked, this, &MainWindow::onStartGameClicked);
    connect(performButton, &QPushButton::clicked, this, &MainWindow::onPerformAction);
    connect(actionBox, &QComboBox::currentTextChanged, this, &MainWindow::onActionSelected);
}

void MainWindow::onAddPlayerClicked() {
    bool ok;
    QString name = QInputDialog::getText(this, "Enter Player Name", "Name:", QLineEdit::Normal, "", &ok);
    if (ok && !name.isEmpty()) {
        if (!game) game = new coup::Game();
        game->create_player(name.toStdString());
        updateUI();
    }
}

void MainWindow::onStartGameClicked() {
    if (!game || game->getPlayers().empty()) {
        QMessageBox::warning(this, "Error", "No players to start the game.");
        return;
    }
    current = game->current_player();
    addPlayerBtn->setEnabled(false);
    startGameBtn->setEnabled(false);
    updateUI();
    updateActions();
    updateTargets();
}

void MainWindow::onPerformAction() {
    if (!game || !current) return;
    std::string action = actionBox->currentText().toStdString();
    std::string targetName = targetBox->currentText().toStdString();
    try {
        coup::Player *target = nullptr;
        if (!targetName.empty()) {
            for (auto &p : game->getPlayers()) {
                if (p->getName() == targetName) {
                    target = p;
                    break;
                }
            }
        }
        if (target) {
    game->play_action(action, target->getName());
} else {
    game->play_action(action);
}
if (game->is_game_over()) {
            QMessageBox::information(this, "Game Over",
                QString::fromStdString("The winner is: " + game->winner()));
            performButton->setEnabled(false);
            actionBox->setEnabled(false);
            targetBox->setEnabled(false);
            return;
        }

        current = game->current_player();
        updateUI();
        updateActions();
        updateTargets();
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void MainWindow::onActionSelected(const QString &action) {
    Q_UNUSED(action);
    updateTargets();
}

void MainWindow::updateUI() {
    if (!game) return;
    playerList->clear();
    for (const auto &p : game->getPlayers()) {
        QString status = p->isAlive() ? (p->isTemporarilyDead() ? "[Temp Dead]" : "[Alive]") : "[Dead]";
        QString display = QString::fromStdString(p->getName()) +
                  " (" + QString::fromStdString(p->roleName()) + ") " +
                  status + " - Coins: " + QString::number(p->coins());
playerList->addItem(display);

    }
    if (current) {
        currentPlayerLabel->setText(QString("Current Player: ") + QString::fromStdString(current->getName()));
    }
}

void MainWindow::updateActions() {
    if (!game || !current) return;
    actionBox->clear();
    if (current->coins() >= 7) actionBox->addItem("Coup");
    if (current->coins() >= 4) actionBox->addItem("Bribe");
    actionBox->addItem("Gather");
    actionBox->addItem("Tax");
    actionBox->addItem("Sanction");
    actionBox->addItem("Arrest");
    for (const auto &s : current->getSpecialAbilities()) {
        actionBox->addItem(QString::fromStdString(s));
    }
}

void MainWindow::updateTargets() {
    if (!game) return;
    targetBox->clear();
    for (const auto &p : game->getPlayers()) {
        if (p != current && p->isAlive()) {
    targetBox->addItem(QString::fromStdString(p->getName()));
} 

    }
}
MainWindow::~MainWindow() = default;

} // namespace coup_gui

