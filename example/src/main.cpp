
#include "ofMain.h"
#include "ofxGumbo.h"

static const char* find_title(const GumboNode* root) {
  assert(root->type == GUMBO_NODE_ELEMENT);
  assert(root->v.element.children.length >= 2);

  const GumboVector* root_children = &root->v.element.children;
  GumboNode* head = NULL;
  for (int i = 0; i < root_children->length; ++i) {
    GumboNode* child = (GumboNode*)(root_children->data[i]);
    if (child->type == GUMBO_NODE_ELEMENT &&
        child->v.element.tag == GUMBO_TAG_HEAD) {
      head = child;
      break;
    }
  }
  assert(head != NULL);

  GumboVector* head_children = &head->v.element.children;
  for (int i = 0; i < head_children->length; ++i) {
    GumboNode* child = (GumboNode*)(head_children->data[i]);
    if (child->type == GUMBO_NODE_ELEMENT &&
        child->v.element.tag == GUMBO_TAG_TITLE) {
      if (child->v.element.children.length != 1) {
        return "<empty title>";
      }
      GumboNode* title_text = (GumboNode*)(child->v.element.children.data[0]);
      assert(title_text->type == GUMBO_NODE_TEXT || title_text->type == GUMBO_NODE_WHITESPACE);
      return title_text->v.text.text;
    }
  }
  return "<no title found>";
}

int main(int argc, char** argv){
    ofHttpResponse request = ofLoadURL("https://forum.openframeworks.cc/");
    auto input = request.data.getText();

    GumboOutput* output = gumbo_parse_with_options(&kGumboDefaultOptions, input.c_str(), input.size());
    const char* title = find_title(output->root);
    ofLogNotice() << title;
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return 0;
}