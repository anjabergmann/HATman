========================================================================
    BIBLIOTECA DE VÍNCULOS DINÁMICOS: Información general del proyecto libGladiator
========================================================================

El Asistente para aplicaciones ha creado este archivo DLL de libGladiator.  
Archivo que contiene un resumen del contenido de los archivos que
componen la aplicación de libGladiator.


libGladiator.vcproj
    Archivo de proyecto principal para proyectos de VC++ generados con el Asistente para aplicaciones. 
    Contiene información acerca de la versión de Visual C++ que generó el archivo y 
    de las plataformas, configuraciones y características del proyecto
    seleccionadas con el Asistente para aplicaciones.

libGladiator.cpp
    Archivo DLL de código fuente principal

	Cuando este archivo DLL se crea, no exporta símbolos. Por lo tanto, 
	no creará un archivo .lib cuando se genere. Si desea que este proyecto 
	constituya una dependencia de otro proyecto, es preciso 
	agregar código para exportar algunos símbolos del archivo DLL para crear 
	una biblioteca de exportación, o establecer la propiedad Omitir biblioteca de entrada en Sí, 
	en la página de propiedades General de la carpeta Vinculador del cuadro de diálogo 
	Páginas de propiedades del proyecto.

/////////////////////////////////////////////////////////////////////////////
Otros archivos estándar:

StdAfx.h, StdAfx.cpp
    Archivos utilizados para generar un archivo de encabezado precompilado (PCH)
    llamado libGladiator.pch, así como un archivo de tipos precompilado llamado StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Notas adicionales:

El asistente para aplicaciones utiliza los comentarios "TODO:" para indicar las partes del código fuente que
se deben agregar o personalizar.

/////////////////////////////////////////////////////////////////////////////
