#include "php.h"
#include "php_gdal.h"
#include <ogr_core.h>
#include <cpl_conv.h>
#include <cpl_string.h>
#include "ogrexception.h"
#include "ogrspatialreference.h"
#include "ogrgeometry.h"
#include "ogrgeometryfactory.h"

zend_class_entry *gdal_ogrgeometryfactory_ce;
zend_object_handlers ogrgeometryfactory_object_handlers;


void ogrgeometryfactory_free_storage(void *object TSRMLS_DC)
{
  php_ogrgeometryfactory_object *obj = (php_ogrgeometryfactory_object *)object;
  //delete obj->geometry; // TODO
  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);
  efree(obj);
}


zend_object_value ogrgeometryfactory_create_handler(zend_class_entry *type TSRMLS_DC)
{
  zval *tmp;
  zend_object_value retval;

  php_ogrgeometryfactory_object *obj =
    (php_ogrgeometryfactory_object *)emalloc(sizeof(php_ogrgeometryfactory_object));
  memset(obj, 0, sizeof(php_ogrgeometryfactory_object));
  obj->std.ce = type;

  ALLOC_HASHTABLE(obj->std.properties);
  zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
#if PHP_VERSION_ID < 50399
  zend_hash_copy(obj->std.properties, &type->default_properties,
                 (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
#else
  object_properties_init(&obj->std, type);
#endif

  retval.handle = zend_objects_store_put(obj, NULL,
                                         ogrgeometryfactory_free_storage, NULL TSRMLS_CC);
  retval.handlers = &ogrgeometryfactory_object_handlers;

  return retval;
}

zend_function_entry ogrgeometryfactory_methods[] = {
  {NULL, NULL, NULL}
};


void php_gdal_ogrgeometryfactory_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "OGRGeometryFactory", ogrgeometryfactory_methods);
  gdal_ogrgeometryfactory_ce = zend_register_internal_class(&ce TSRMLS_CC);
  gdal_ogrgeometryfactory_ce->create_object = ogrgeometryfactory_create_handler;
  memcpy(&ogrgeometryfactory_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  ogrgeometryfactory_object_handlers.clone_obj = NULL;
}


/* VIM settings */
/* ex: set tabstop=2 expandtab shiftwidth=2 smartindent */