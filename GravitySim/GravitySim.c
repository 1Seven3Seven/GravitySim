#define PY_SSIZE_T_CLEAN
#include "C:\\Users\\pburg\\AppData\\Local\\Programs\\Python\\Python311\\include\\Python.h"

#include "PyForceAngle.c"

static PyModuleDef GravitySimModule = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "GravitySim",
    .m_doc = PyDoc_STR("A library to simulate gravity."),
    .m_size = -1
};

PyMODINIT_FUNC
PyInit_GravitySim(void) {
  PyObject *module;
  
  if (PyType_Ready(&PyForceAngleType) < 0)
  {
    return NULL;
  }
  
  module = PyModule_Create(&GravitySimModule);
  if (module == NULL)
  {
    return NULL;
  }
  
  if (PyModule_AddObjectRef(module, "ForceAngle", (PyObject *) &PyForceAngleType) < 0)
  {
    Py_DECREF(module);
    return NULL;
  }
  
  return module;
}
