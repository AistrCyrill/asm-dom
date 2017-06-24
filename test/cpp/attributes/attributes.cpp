#include "../../../src/asm-dom.hpp"
#include "../utils.hpp"
#include <emscripten/bind.h>
#include <emscripten/val.h>

using namespace asmdom;

void shouldHaveTheirProvidedValues() {
	VNode* vnode = new VNode("div", new VNodeData(
		VNodeAttrs {
			{"href", "/foo"},
			{"minlength", "1"},
			{"value", "true"}
		}
	));
	patch(getRoot(), vnode);
	
	emscripten::val elm = getBodyFirstChild();

	assertEquals(
		elm.call<emscripten::val>("getAttribute", emscripten::val("href")),
		emscripten::val("/foo")
	);
	assertEquals(
		elm.call<emscripten::val>("getAttribute", emscripten::val("minlength")),
		emscripten::val("1")
	);
	assertEquals(
		elm.call<emscripten::val>("getAttribute", emscripten::val("value")),
		emscripten::val("true")
	);

	deleteVNode(vnode);
};

void attributesCanBeMemoized() {
	VNodeData* data = new VNodeData(
		VNodeAttrs {
			{"href", "/foo"},
			{"minlength", "1"},
			{"value", "true"}
		}
	);
	VNode* vnode = new VNode("div", data);
	VNode* vnode2 = new VNode("div", data);
	patch(getRoot(), vnode);
	
	emscripten::val elm = getBodyFirstChild();

	assertEquals(
		elm.call<emscripten::val>("getAttribute", emscripten::val("href")),
		emscripten::val("/foo")
	);
	assertEquals(
		elm.call<emscripten::val>("getAttribute", emscripten::val("minlength")),
		emscripten::val("1")
	);
	assertEquals(
		elm.call<emscripten::val>("getAttribute", emscripten::val("value")),
		emscripten::val("true")
	);
	patch(vnode, vnode2);
	
	elm = getBodyFirstChild();

	assertEquals(
		elm.call<emscripten::val>("getAttribute", emscripten::val("href")),
		emscripten::val("/foo")
	);
	assertEquals(
		elm.call<emscripten::val>("getAttribute", emscripten::val("minlength")),
		emscripten::val("1")
	);
	assertEquals(
		elm.call<emscripten::val>("getAttribute", emscripten::val("value")),
		emscripten::val("true")
	);

	deleteVNode(vnode2);
};

void shouldBeOmittedWhenFalsyValuesAreProvided() {
	VNode* vnode = new VNode("div", new VNodeData(
		VNodeAttrs {
			{"href", "null"},
			{"minlength", "0"},
			{"value", "false"}
		}
	));
	patch(getRoot(), vnode);
	
	emscripten::val elm = getBodyFirstChild();

	assertEquals(
		elm.call<emscripten::val>("getAttribute", emscripten::val("href")),
		emscripten::val("null")
	);
	assertEquals(
		elm.call<emscripten::val>("getAttribute", emscripten::val("minlength")),
		emscripten::val("0")
	);
	assertEquals(
		elm.call<emscripten::val>("getAttribute", emscripten::val("value")),
		emscripten::val::null()
	);

	deleteVNode(vnode);
};

void shouldBeSetCorrectlyWhenNamespaced() {
	VNode* vnode = new VNode("div", new VNodeData(
		VNodeAttrs {
			{"xlink:href", "#foo"}
		}
	));
	patch(getRoot(), vnode);
	
	emscripten::val elm = getBodyFirstChild();

	assertEquals(
		elm.call<emscripten::val>("getAttributeNS",
			emscripten::val("http://www.w3.org/1999/xlink"),
			emscripten::val("href")
		),
		emscripten::val("#foo")
	);

	deleteVNode(vnode);
};

EMSCRIPTEN_BINDINGS(attributes_tests) {
  emscripten::function("shouldHaveTheirProvidedValues", &shouldHaveTheirProvidedValues);
  emscripten::function("attributesCanBeMemoized", &attributesCanBeMemoized);
  emscripten::function("shouldBeOmittedWhenFalsyValuesAreProvided", &shouldBeOmittedWhenFalsyValuesAreProvided);
  emscripten::function("shouldBeSetCorrectlyWhenNamespaced", &shouldBeSetCorrectlyWhenNamespaced);
};