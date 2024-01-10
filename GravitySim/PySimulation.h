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
PySimulation_item(PyObject *self, Py_ssize_t i)
{
    PySimulation *object = (PySimulation *)self;

    if (i < 0 || i >= object->simulation.num_bodies)
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

    py_celestial_body->celestial_body = object->simulation.bodies[i];

    return (PyObject *)py_celestial_body;
}

static int
/*
 * Set the CelestialBody object at the given index.
 */
PySimulation_ass_item(PyObject *self, Py_ssize_t i, PyObject *value)
{
    PySimulation *object = (PySimulation *)self;

    if (i < 0 || i >= object->simulation.num_bodies)
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

    object->simulation.bodies[i] = new_body->celestial_body;

    return 0;
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

static PyTypeObject PySimulationType = {
        .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "GravitySim.Simulation",
        .tp_basicsize = sizeof(PySimulation),
        .tp_itemsize = 0,
        .tp_flags = Py_TPFLAGS_DEFAULT,
        .tp_doc = PyDoc_STR("Simulation object."),
        .tp_members = PySimulation_members,
        .tp_as_sequence = &PySimulation_as_sequence,
        .tp_init = (initproc)PySimulation_init,
        .tp_new = PyType_GenericNew,
        .tp_dealloc = (destructor)PySimulation_dealloc,
};


#endif //GRAVITYSIM_PYSIMULATION_H
