
system_compressor = library("system_compressor.dsp");

compressor_mono(ratio,thresh,att,rel,x) = cgm*x with {
    cgm = system_compressor.compression_gain_mono(ratio,thresh,att,rel,abs(x));
};

compressor_mono_demo = compressor with{
    compressor = _
    : compressor_mono(system_compressor.ratio,
                      system_compressor.threshold,
                      system_compressor.attack,
                      system_compressor.release)
    : *(system_compressor.outgain)
    ;   
};


process = compressor_mono_demo;

