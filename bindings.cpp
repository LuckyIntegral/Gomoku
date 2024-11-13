#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Game.hpp"

namespace py = pybind11;

PYBIND11_MODULE(game_module, m) {
    py::class_<Game>(m, "Game")
        .def(py::init<>())
        .def("getBoard", &Game::getBoard)
        .def("makeMove", &Game::makeMove)
        .def("undoMove", &Game::undoMove)
        .def("evaluateBoard", &Game::evaluateBoard)
        .def("getBestPossibleMoves", &Game::getBestPossibleMoves)
        .def("isValidMove", &Game::isValidMove)
        .def("heuristicEvaluation", &Game::heuristicEvaluation);
}