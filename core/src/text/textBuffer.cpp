#include "textBuffer.h"

TextBuffer::TextBuffer(FONScontext* _fsContext) : m_fsContext(_fsContext) {
    m_dirty = false;
    m_bound = false;
}

void TextBuffer::init(int _size) {
    glfonsBufferCreate(m_fsContext, _size, &m_fsBuffer);
}

TextBuffer::~TextBuffer() {
    glfonsBufferDelete(m_fsContext, m_fsBuffer);

    if(m_transform) {
        m_transform->destroy();
    }
}

void TextBuffer::setTextureTransform(std::unique_ptr<Texture> _texture) {
    m_transform = std::move(_texture);
}

std::shared_ptr<Texture> TextBuffer::getTextureTransform() const {
    return m_transform;
}

bool TextBuffer::getVertices(std::vector<float>* _vertices, int* _nVerts) {
    bool res;

    bind();
    res = glfonsVertices(m_fsContext, _vertices, _nVerts);
    unbind();

    return res;
}

void TextBuffer::expand() {
    if (m_transform) {
        bind();
        // expand the transform texture in cpu side
        glfonsExpandTransform(m_fsContext, m_fsBuffer, m_transform->getWidth() * 2);
        // double size of texture
        m_transform->resize(m_transform->getWidth() * 2, m_transform->getHeight() * 2);
        unbind();
    }
}

void TextBuffer::bind() {
    if (!m_bound) {
        m_bound = true;
        glfonsBindBuffer(m_fsContext, m_fsBuffer);
    }
}

fsuint TextBuffer::genTextID() {
    fsuint id;

    bind();
    glfonsGenText(m_fsContext, 1, &id);
    unbind();
    
    return id;
}
    
void TextBuffer::rasterize(const std::string& _text, fsuint _id) {
    bind();
    glfonsRasterize(m_fsContext, _id, _text.c_str(), FONS_EFFECT_NONE /* TODO : update signature, remove effect */);
    unbind();
}

void TextBuffer::triggerTransformUpdate() {
    if (m_dirty) {
        bind();
        glfonsUpdateTransforms(m_fsContext, (void*) this);
        unbind();
        m_dirty = false;
    }
}

void TextBuffer::transformID(fsuint _textID, float _x, float _y, float _rot, float _alpha) {
    bind(); 
    glfonsTransform(m_fsContext, _textID, _x, _y, _rot, _alpha);
    unbind();

    m_dirty = true;
}

void TextBuffer::unbind() {
    if (m_bound) {
        glfonsBindBuffer(m_fsContext, 0);
        m_bound = false;
    }
}
