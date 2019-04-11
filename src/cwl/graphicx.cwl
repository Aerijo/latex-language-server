# graphicx package
# mario 09/06/2007
# URL: http://tug.ctan.org/tex-archive/macros/latex/required/graphics/
\DeclareGraphicsExtensions{ext-list}
\DeclareGraphicsRule{ext}{type}{read-file}{command%plain}
\graphicspath{dir-list}
\includegraphics*[keyvals]{imagefile}
\includegraphics[keyvals]{imagefile}
\reflectbox{text}
\resizebox*{h-length}{v-length}{text}
\resizebox{h-length}{v-length}{text}
\rotatebox[keyvals]{angle}{text}
\scalebox{h-scale}{text}
\scalebox{h-scale}[v-scale]{text}
\setkeys{Gin}{key val}
#keyvals:\includegraphics
width=##L
height=##L
bb=
bbllx=
bbly=
bburx=
bbury=
natwidth=##L
natheight=##L
hiresbb#true,false
angle=
origin=
trim=
viewport=
totalheight=##L
scale=
clip#true,false
draft#true,false
type=
ext=
read=
command=
keepaspectratio
#endkeyvals
#keyvals:\rotatebox
origin=
x=
y=
units=
#endkeyvals
