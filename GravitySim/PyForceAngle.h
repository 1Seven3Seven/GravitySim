#ifndef GRAVITYSIM_PYFORCEANGLE_H
#define GRAVITYSIM_PYFORCEANGLE_H

// Python Libraries
#define PY_SSIZE_T_CLEAN

#include "C:\\Users\\pburg\\AppData\\Local\\Programs\\Python\\Python311\\include\\Python.h"
#include <C:\\Users\\pburg\\AppData\\Local\\Programs\\Python\\Python311\\include\\structmember.h>

// C Libraries
#include <stddef.h> /* for offsetof() */

// My stuff
#include "ForceAngle.h"

typedef struct _PyForceAngle
{
    PyObject_HEAD

    ForceAngle force_angle;
} PyForceAngle;

static int
PyForceAngle_init(PyForceAngle *self, PyObject *args, PyObject *Py_UNUSED(ignored))
{
    int parse_result = PyArg_ParseTuple(args, "dd", &self->force_angle.force, &self->force_angle.angle);

    if (!parse_result)
    {
        return -1;
    }

    return 0;
}

static PyMemberDef PyForceAngle_members[] = {
        {"force", T_DOUBLE, offsetof(PyForceAngle, force_angle) + offsetof(ForceAngle, force), READONLY, NULL},
        {"angle", T_DOUBLE, offsetof(PyForceAngle, force_angle) + offsetof(ForceAngle, angle), READONLY, NULL},
        {NULL}
};

static PyTypeObject PyForceAngleType = {
        .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "GravitySim.ForceAngle",
        .tp_basicsize = sizeof(PyForceAngle),
        .tp_itemsize = 0,
        .tp_flags = Py_TPFLAGS_DEFAULT,
        .tp_doc = PyDoc_STR("Force Angle object."),
        .tp_members = PyForceAngle_members,
        .tp_init = (initproc)PyForceAngle_init,
        .tp_new = PyType_GenericNew,
};

#endif