#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Game.hpp"
#include "AI.hpp"

namespace py = pybind11;

PYBIND11_MODULE(game_module, m, py::mod_gil_not_used()) {
    m.doc() = "game_module plugin";
    py::class_<Game>(m, "Game")
        .def(py::init<>())
        .def("get_board", &Game::get_board)
        .def("make_move", &Game::make_move)
        .def("undo_move", &Game::undo_move)
        .def("evaluate_board", &Game::evaluate_board)
        .def("get_best_possible_moves", &Game::get_best_possible_moves)
        .def("is_valid_move", &Game::is_valid_move)
        .def("heuristic_evaluation", &Game::heuristic_evaluation)
        .def("get_captures", &Game::get_captures)
        .def("is_win", &Game::is_win);

    py::class_<AI>(m, "AI")
        .def(py::init<Game&, int>())
        .def("iterative_deepening", &AI::iterative_deepening);
}
