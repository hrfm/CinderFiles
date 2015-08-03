#version 120

// Textures
uniform sampler2D tex;
uniform float     time;
uniform vec2      resolution;
uniform vec2      matrix; // Segments of fontMatrix.
uniform float     segments; // Segments of fontMatrix.

vec2 cells(){
    return matrix * segments;
}

vec2 stepOfMtx(){
    return vec2( ( resolution.x / matrix.x ) / resolution.x, ( resolution.y / matrix.y ) / resolution.y );
}

vec2 stepOfCell(){
    vec2 cell = cells();
    return vec2( ( resolution.x / cell.x ) / resolution.x, ( resolution.y / cell.y ) / resolution.y );
}

void main(void){
    
    vec2 posOrigin  = gl_FragCoord.xy / resolution;
    vec2 cell       = cells();
    
    // - matrix 上の所属座標を調べる
    vec2 stepOfMtx = stepOfMtx();
    vec2 posOfMtx  = gl_FragCoord.xy / resolution;
    posOfMtx.x = posOfMtx.x - mod( posOfMtx.x, stepOfMtx.x ) + stepOfMtx.x * 0.5;
    posOfMtx.y = posOfMtx.y - mod( posOfMtx.y, stepOfMtx.y ) + stepOfMtx.y * 0.5;
    
    // - matrix 上の所属座標の色を取得する
    vec4 color   = texture2D( tex, posOfMtx );
    float chroma = color.x * 0.3 + color.y * 0.59 + color.z * 0.11; // 明るさを計算
    color.xyz    = color.xyz + ( vec3(chroma) - color.xyz ) * 0.8;
    
    // - matrix * cell のどの箇所の物か調べる.
    vec2 stepOfCell = stepOfCell();
    vec2 posOfCell  = gl_FragCoord.xy / resolution;
    posOfCell.x = posOfCell.x - mod( posOfCell.x, stepOfCell.x ) + stepOfCell.x * 0.5;
    posOfCell.y = posOfCell.y - mod( posOfCell.y, stepOfCell.y ) + stepOfCell.y * 0.5;
    
    // - matrix * segments で分解した場合にどこに所属するかを調べる.
    float col    = floor( posOfCell.x / 1.0f * cell.x );
    float row    = floor( posOfCell.y / 1.0f * cell.y );
    
    float col2   = mod( col, segments );
    float row2   = mod( row, segments );
    
    float center = floor( segments / 2.0f );
    
    // - 色を取得する.
    
    gl_FragColor = texture2D( tex, posOfMtx );
    
}