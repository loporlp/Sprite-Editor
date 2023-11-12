#include "controller.h"
#include "mainwindow.h"
#include "model.h"

Controller::Controller(Model &model, MainWindow &view)
    : model(model)
    , view(view)
{}
