#ifndef GRAVITYSIM_PYSIMULATION_H
#define GRAVITYSIM_PYSIMULATION_H

// Python Libraries
#define PY_SSIZE_T_CLEAN

#include <C:\\Users\\pburg\\AppData\\Local\\Programs\\Python\\Python311\\include\\Python.h>
#include <C:\\Users\\pburg\\AppData\\Local\\Programs\\Python\\Python311\\include\\structmember.h>

// C Libraries
#include <stddef.h> /* for offsetof() */

// My stuff
#include "Simulation.h"
#include "PyCelestialBody.h"

typedef struct _PySimulation
{
    PyObject_HEAD

    Simulation simulation;
} PySimulation;

static void
PySimulation_dealloc(PySimulation *self)
{
    simulation_de_init(&self->simulation);

    Py_TYPE(self)->tp_free((PyObject *)self);
}

static int
PySimulation_init(PySimulation *self, PyObject *args, PyObject *Py_UNUSED(ignored))
{
    unsigned int number_bodies;

    int parse_result = PyArg_ParseTuple(args, "I", &number_bodies);

    if (!parse_result)
    {
        return -1;
    }

    simulation_init(&self->simulation);
    int setup_result = simulation_setup(&self->simulation, number_bodies);

    if (setup_result)
    {
        PyErr_SetString(PyExc_RuntimeError, "Failed to set up simulation.");
        return -1;
    }

    return 0;
}

static PyObject *
/*
 * Get the CelestialBody object at the given index.
 */
PySimulation_item(PySimulation *self, Py_ssize_t i)
{
    if (i < 0 || i >= self->simulation.num_bodies)
    {
        PyErr_SetString(PyExc_IndexError, "Index out of range.");
        return NULL;
    }

    PyCelestialBody *py_celestial_body = PyObject_New(PyCelestialBody, &PyCelestialBodyType);
    if (!py_celestial_body)
    {
        PyErr_SetString(PyExc_RuntimeError, "Failed to create CelestialBody object.");
        return NULL;
    }

    py_celestial_body->celestial_body = self->simulation.bodies[i];

    return (PyObject *)py_celestial_body;
}

static int
/*
 * Set the CelestialBody object at the given index.
 */
PySimulation_ass_item(PySimulation *self, Py_ssize_t i, PyObject *value)
{
    if (i < 0 || i >= self->simulation.num_bodies)
    {
        PyErr_SetString(PyExc_IndexError, "Index out of range.");
        return -1;
    }

    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Item deletion not supported.");
        return -1;
    }

    if (!PyObject_TypeCheck(value, &PyCelestialBodyType))
    {
        PyErr_SetString(PyExc_TypeError, "Expected a CelestialBody object.");
        return -1;
    }

    PyCelestialBody *new_body = (PyCelestialBody *)value;

    self->simulation.bodies[i] = new_body->celestial_body;

    return 0;
}

static PyObject *
/*
 * Get the x and y coordinates of the CelestialObject at the given index.
 */
PySimulation_get_coordinates_of(PySimulation *self, PyObject *args, PyObject *Py_UNUSED(ignored))
{
    unsigned int index;
    unsigned int parse_result = PyArg_ParseTuple(args, "I", &index);

    if (!parse_result)
    {
        PyErr_SetString(PyExc_TypeError, "Invalid input argument.");
        return NULL;
    }

    if (index >= self->simulation.num_bodies)
    {
        PyErr_SetString(PyExc_IndexError, "Index out of range.");
        return NULL;
    }

    PyObject *tuple = Py_BuildValue("(dd)",
                                    self->simulation.bodies[index].x_position,
                                    self->simulation.bodies[index].y_position);

    if (!tuple)
    {
        PyErr_SetString(PyExc_RuntimeError, "Failed to create tuple object.");
        return NULL;
    }

    return tuple;
}

static PyObject *
/*
 * Performs a single step over the given time slice.
 */
PySimulation_step(PySimulation *self, PyObject *args, PyObject *Py_UNUSED(ignored))
{
    double delta_time, gravitational_constant;

    int parse_result = PyArg_ParseTuple(args, "dd", &delta_time, &gravitational_constant);

    if (!parse_result)
    {
        PyErr_SetString(PyExc_TypeError, "Invalid inapt argument");
        return NULL;
    }

    // Zero the acceleration
    // This is instantaneous acceleration and is calculated separately each step
    for (unsigned int i = 0; i < self->simulation.num_in_use_bodies; i++)
    {
        self->simulation.bodies[i].x_acceleration = 0;
        self->simulation.bodies[i].y_acceleration = 0;
    }

    simulation_calculate_and_apply_forces(
        &self->simulation,
        gravitational_constant
    );

    simulation_update_positions(
        &self->simulation,
        delta_time
    );

    Py_RETURN_NONE;
}

static PyMemberDef PySimulation_members[] = {
    {
        .name = "num_bodies",
        .type = T_UINT,
        .offset = offsetof(PySimulation, simulation) + offsetof(Simulation, num_bodies),
        .flags = READONLY,
        .doc = NULL
    },
};

static PySequenceMethods PySimulation_as_sequence = {
    .sq_length = NULL,
    .sq_item = PySimulation_item,
    .sq_ass_item = PySimulation_ass_item
};

static PyMethodDef PySimulation_methods[] = {
    {
        .ml_name = "get_coordinates_of",
        .ml_meth = (PyCFunction)PySimulation_get_coordinates_of,
        .ml_flags = METH_VARARGS,
        .ml_doc = NULL
    },
    {
        .ml_name = "step",
        .ml_meth = (PyCFunction)PySimulation_step,
        .ml_flags = METH_VARARGS,
        .ml_doc = NULL
    },
    {NULL}
};

static PyTypeObject PySimulationType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "GravitySim.Simulation",
    .tp_basicsize = sizeof(PySimulation),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("Simulation object."),
    .tp_methods = PySimulation_methods,
    .tp_members = PySimulation_members,
    .tp_as_sequence = &PySimulation_as_sequence,
    .tp_init = (initproc)PySimulation_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = (destructor)PySimulation_dealloc,
};


#endif //GRAVITYSIM_PYSIMULATION_H
