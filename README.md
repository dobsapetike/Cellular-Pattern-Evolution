<h1>Cellular Pattern Evolution</h1>

<h4>About</h4>

Using the CMA-ES evolutionary algorithm for continuous optimization, the core idea of this master thesis was to find the parameters (weights) of a multi-layer perceptron neural net, which encodes the update rule of a non-regular cellular automaton (updating its state as well as its structure), for a raster approximation of a user-defined vector  image. The goal was to implement a modular framework with emphasis on extensibility which allows convenient experimentation, and at the same time to improve on the previous research in the field in regard to cell representation (using rectangular shapes and Voronoi regions as geometric structures of the cells) and topological update methods - check the  <a href="https://github.com/dobsapetike/Cellular-Pattern-Evolution/blob/master/msc_praca.pdf">thesis paper</a> for more detail (only Slovak). 

<h4>External dependencies C++</h4>

<a href="http://sourceforge.net/projects/tinyxml/">TinyXML 2.6.2 - XML parser</a><br/>
<a href="http://www.boost.org/users/history/version_1_59_0.html">Boost 1.59.0</a><br/>
<a href="http://image.diku.dk/shark/sphinx_pages/build/html/rest_sources/downloads/downloads.html">SHARK machine learning 2.8 - MO-CMAES</a></br> 
<a href="http://www.gnuplot.info/download.html">gnuplot (has to be installed)</a></br> 
<a href="http://freeimage.sourceforge.net/">FreeImage 3.17.0</a></br> 
