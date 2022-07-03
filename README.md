# Mini
Mini /ˈmɪni / 是从 node.js 中抽离出来 V8 库，主要用途是个人探究 V8 库接口 和 Node.js 的结构。Mini 澄清了Node.js 如何以 gyp 依赖形式包含 V8 的问题。最终目标可以实现编写只有部分 Node.js 能力的 Mini 版。 

```bash
./configure.py
make -C out -j4
```