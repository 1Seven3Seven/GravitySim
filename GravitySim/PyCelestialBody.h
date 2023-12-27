#ifndef GRAVITYSIM_PYCELESTIALBODY_H
#define GRAVITYSIM_PYCELESTIALBODY_H

// Python Libraries
#define PY_SSIZE_T_CLEAN

#include "C:\\Users\\pburg\\AppData\\Local\\Programs\\Python\\Python311\\include\\Python.h"
#include <C:\\Users\\pburg\\AppData\\Local\\Programs\\Python\\Python311\\include\\structmember.h>
#include <C:\\Users\\pburg\\AppData\\Local\\Programs\\Python\\Python311\\include\\pyerrors.h>

// C Libraries
#include <stddef.h> /* for offsetof() */

// My stuff
#include "CelestialBody.h"
#include "PyForceAngle.h"

extern PyTypeObject PyCelestialBodyType;

typedef struct _PyCelestialBody
{
    PyObject_HEAD

    CelestialBody celestial_body;
} PyCelestialBody;

static int
PyCelestialBody_init(PyCelestialBody *self, PyObject *args, PyObject *Py_UNUSED(ignored))
{
    int parse_result = PyArg_ParseTuple(args, "Idd",
                                        &self->celestial_body.mass,
                                        &self->celestial_body.x_position,
                                        &self->celestial_body.y_position);

    if (!parse_result)
    {
        return -1;
    }

    return 0;
}

static PyObject *
PyCelestialBody_apply_velocity(PyCelestialBody *self, PyObject *args, PyObject *Py_UNUSED(ignored))
{
    double delta_time;

    int parse_result = PyArg_ParseTuple(args, "d", &delta_time);

    if (!parse_result)
    {
        PyErr_SetString(PyExc_TypeError, "Invalid inapt argument");
        return NULL;
    }

    apply_velocity(&self->celestial_body, delta_time);

    Py_RETURN_NONE;
}

static PyObject *
PyCelestialBody_apply_acceleration(PyCelestialBody *self, PyObject *args, PyObject *Py_UNUSED(ignored))
{
    double delta_time;

    int parse_result = PyArg_ParseTuple(args, "d", &delta_time);

    if (!parse_result)
    {
        PyErr_SetString(PyExc_TypeError, "Invalid inapt argument");
        return NULL;
    }

    apply_acceleration(&self->celestial_body, delta_time);

    Py_RETURN_NONE;
}

static PyObject *
PyCelestialBody_calculate_force_between(PyCelestialBody *self, PyObject *args, PyObject *Py_UNUSED(ignored))
{
    PyObject *py_object_other;
    double gravitational_constant;

    int parse_result = PyArg_ParseTuple(args, "Od", &py_object_other, &gravitational_constant);

    if (!parse_result)
    {
        PyErr_SetString(PyExc_TypeError, "Invalid inapt arguments");
        return NULL;
    }

    if (!PyObject_TypeCheck(py_object_other, &PyCelestialBodyType))
    {
        PyErr_SetString(PyExc_TypeError, "Argument 1 must be a CelestialBody");
        return NULL;
    }

    PyCelestialBody *other = (PyCelestialBody *)py_object_other;

    ForceAngle force_angle = calculate_force_between(&self->celestial_body, &other->celestial_body,
                                                     gravitational_constant);

    PyForceAngle *py_force_angle = PyObject_New(PyForceAngle, &PyForceAngleType);
    if (!py_force_angle)
    {
        return NULL;
    }

    py_force_angle->force_angle = force_angle;

    return (PyObject *)py_force_angle;
}

static PyObject *
PyCelestialBody_apply_force(PyCelestialBody *self, PyObject *args, PyObject *Py_UNUSED(ignored))
{
    PyObject *py_object_force_angle;

    int parse_result = PyArg_ParseTuple(args, "O", &py_object_force_angle);

    if (!parse_result)
    {
        PyErr_SetString(PyExc_TypeError, "Invalid inapt argument");
        return NULL;
    }

    if (!PyObject_TypeCheck(py_object_force_angle, &PyForceAngleType))
    {
        PyErr_SetString(PyExc_TypeError, "Argument must be a ForceAngle");
        return NULL;
    }

    PyForceAngle *force_angle = (PyForceAngle *)py_object_force_angle;

    apply_force(&self->celestial_body, force_angle->force_angle);

    Py_RETURN_NONE;
}

static PyMemberDef PyCelestialBody_members[] = {
        {
                .name = "mass",
                .type = T_UINT,
                .offset = offsetof(PyCelestialBody, celestial_body) + offsetof(CelestialBody, mass),
                .flags = READONLY,
                .doc = NULL
        },
        {
                .name = "x_position",
                .type = T_DOUBLE,
                .offset = offsetof(PyCelestialBody, celestial_body) + offsetof(CelestialBody, x_position),
                .flags = READONLY,
                .doc = NULL
        },
        {
                .name = "y_position",
                .type = T_DOUBLE,
                .offset = offsetof(PyCelestialBody, celestial_body) + offsetof(CelestialBody, y_position),
                .flags = READONLY,
                .doc = NULL
        },
        {
                .name = "x_velocity",
                .type = T_DOUBLE,
                .offset = offsetof(PyCelestialBody, celestial_body) + offsetof(CelestialBody, x_velocity),
                .flags = READONLY,
                .doc = NULL
        },
        {
                .name = "y_velocity",
                .type = T_DOUBLE,
                .offset = offsetof(PyCelestialBody, celestial_body) + offsetof(CelestialBody, y_velocity),
                .flags = READONLY,
                .doc = NULL
        },
        {
                .name = "x_acceleration",
                .type = T_DOUBLE,
                .offset = offsetof(PyCelestialBody, celestial_body) + offsetof(CelestialBody, x_acceleration),
                .flags = READONLY,
                .doc = NULL
        },
        {
                .name = "y_acceleration",
                .type = T_DOUBLE,
                .offset = offsetof(PyCelestialBody, celestial_body) + offsetof(CelestialBody, y_acceleration),
                .flags = READONLY,
                .doc = NULL
        },
        {NULL}
};

static PyMethodDef PyCelestialBody_methods[] = {
        {
                .ml_name = "apply_velocity",
                .ml_meth = (PyCFunction)PyCelestialBody_apply_velocity,
                .ml_flags = METH_VARARGS,
                .ml_doc = NULL
        },
        {
                .ml_name = "apply_acceleration",
                .ml_meth = (PyCFunction)PyCelestialBody_apply_acceleration,
                .ml_flags = METH_VARARGS,
                .ml_doc = NULL
        },
        {
                .ml_name = "calculate_force_between",
                .ml_meth = (PyCFunction)PyCelestialBody_calculate_force_between,
                .ml_flags = METH_VARARGS,
                .ml_doc = NULL
        },
        {
                .ml_name = "apply_force",
                .ml_meth = (PyCFunction)PyCelestialBody_apply_force,
                .ml_flags = METH_VARARGS,
                .ml_doc = NULL
        },
        {NULL}
};

static PyTypeObject PyCelestialBodyType = {
        .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "GravitySim.CelestialBody",
        .tp_basicsize = sizeof(PyCelestialBody),
        .tp_itemsize = 0,
        .tp_flags = Py_TPFLAGS_DEFAULT,
        .tp_doc = PyDoc_STR("Celestial Body object."),
        .tp_methods = PyCelestialBody_methods,
        .tp_members = PyCelestialBody_members,
        .tp_init = (initproc)PyCelestialBody_init,
        .tp_new = PyType_GenericNew,
};

#endif