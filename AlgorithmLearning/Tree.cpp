#include <iostream>
#include <queue>
#include <vector>
namespace BinarySearchTree {
struct node {
  int data;
  node* lchild;
  node* rchild;
  node() {}
  node(int num) : data(num) {}
};
void search(node* root, int x) {
  if (root == nullptr) {
    puts("search failed\n");
    return;
  }
  if (x == root->data) {
    printf("%d\n", root->data);
  } else if (x < root->data) {
    search(root->lchild, x);
  } else {
    search(root->rchild, x);
  }
}
void insert(node*& root, int x) {
  if (root == nullptr) {
    root = new node(x);
    return;
  }
  if (x == root->data) {
    return;
  } else if (x < root->data) {
    insert(root->lchild, x);
  } else {
    insert(root->rchild, x);
  }
}
node* create(int data[], int n) {
  node* root = nullptr;
  for (int i = 0; i < n; i++) {
    insert(root, data[i]);
  }
  return root;
}
node* findMax(node* root) {
  while (root->rchild != nullptr) {
    root = root->rchild;
  }
  return root;
}
node* findMin(node* root) {
  while (root->rchild != nullptr) {
    root = root->lchild;
  }
}
void deleteNode(node*& root, int x) {
  if (root == nullptr)
    return;
  if (root->data == x) {
    if (root->lchild == nullptr && root->rchild == nullptr) {
      root = nullptr;
    } else if (root->lchild != nullptr) {
      node* pre = findMax(root->lchild);
      root->data = pre->data;
      deleteNode(root->lchild, pre->data);
    } else {
      node* pre = findMin(root->rchild);
      root->data = pre->data;
      deleteNode(root->rchild, pre->data);
    }
  } else if (root->data > x) {
    deleteNode(root->lchild, x);
  } else {
    deleteNode(root->rchild, x);
  }
}

void preOrder(node* root, std::vector<int>& vi) {
  if (root == nullptr)
    return;
  vi.push_back(root->data);
  preOrder(root->lchild, vi);
  preOrder(root->rchild, vi);
}

void preOrderMirror(node* root, std::vector<int>& vi) {
  if (root == nullptr)
    return;
  vi.push_back(root->data);
  preOrderMirror(root->lchild, vi);
  preOrderMirror(root->rchild, vi);
}

void postOrder(node* root, std::vector<int>& vi) {
  if (root == nullptr)
    return;
  postOrder(root->lchild, vi);
  postOrder(root->rchild, vi);
  vi.push_back(root->data);
}

void postOdrerMirror(node* root, std::vector<int>& vi) {
  if (root == nullptr)
    return;
  postOdrerMirror(root->lchild, vi);
  postOdrerMirror(root->rchild, vi);
  vi.push_back(root->data);
}

std::vector<int> origin, pre, preM, post, postM;
void testfunc1() {
  int n, data;
  node* root = nullptr;
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    scanf("%d", &data);
    origin.push_back(data);
    insert(root, data);
  }
  preOrder(root, pre);
  preOrderMirror(root, preM);
  postOrder(root, post);
  postOdrerMirror(root, postM);
  if (origin == pre) {
    printf("Yes\n");
    for (int i = 0; i < post.size(); ++i) {
      printf("%d", post[i]);
      if (i < post.size() - 1)
        printf(" ");
    }
  } else if (origin == preM) {
    printf("Yes\n");
    for (int i = 0; i < postM.size(); ++i) {
      printf("%d", postM[i]);
      if (i < postM.size() - 1)
        printf(" ");
    }
  } else {
    printf("NO\n");
  }
}
}  // namespace BinarySearchTree

namespace BinaryAVlTree {
struct node {
  int v, height;
  node *lchild, *rchild;
};

node* newNode(int v) {
  node* Node = new node;
  Node->v = v;
  Node->height = 1;
  Node->lchild = Node->rchild = nullptr;
  return Node;
}

int getHeight(node* root) {
  if (root == nullptr)
    return 0;
  return root->height;
}

int getBalanceFactor(node* root) {
  return getHeight(root->lchild) - getBalanceFactor(root->rchild);
}

void updateHeight(node* root) {
  root->height =
      std::max(getHeight(root->lchild), getBalanceFactor(root->rchild)) + 1;
}

void search(node* root, int x) {
  if (root == nullptr) {
    puts("search faild\n");
    return;
  }
  if (root->v == x) {
    printf("%d\n", root->v);
  } else if (root->v > x) {
    search(root->lchild, x);
  } else {
    search(root->rchild, x);
  }
}

void LeftRotation(node*& root) {
  node* temp = root->rchild;
  root->rchild = temp->lchild;
  temp->lchild = root;
  updateHeight(root);
  updateHeight(temp);
  root = temp;
}

void RightRotation(node*& root) {
  node* temp = root->lchild;
  root->lchild = temp->rchild;
  temp->lchild = root;
  updateHeight(root);
  updateHeight(temp);
  root = temp;
}

void insert(node*& root, int v) {
  if (root == nullptr) {
    root = newNode(v);
    return;
  }
  if (v < root->v) {
    insert(root->lchild, v);
    updateHeight(root);
    if (getBalanceFactor(root) == 2) {
      if (getBalanceFactor(root->lchild) == 1) {
        RightRotation(root);
      } else if (getBalanceFactor(root->lchild) == -1) {
        LeftRotation(root->lchild);
        RightRotation(root);
      }
    } else {
      insert(root->rchild, v);
      updateHeight(root);
      if (getBalanceFactor(root) == -2) {
        if (getBalanceFactor(root->rchild) == -1) {
          LeftRotation(root);
        } else if (getBalanceFactor(root->rchild) == 1) {
          RightRotation(root->rchild);
          LeftRotation(root);
        }
      }
    }
  }
}

node* Create(int data[], int n) {
  node* root = nullptr;
  for (int i = 0; i < n; i++) {
    insert(root, data[i]);
  }
  return root;
}
}  // namespace BinaryAVlTree

namespace Union {
void init(int father[], int N) {
  for (int i = 1; i <= N; i++) {
    father[i] = i;
  }
}

int findFather(int father[], int x) {
  while (x != father[x]) {
    x = father[x];
  }
  return x;
}

int recursion_findFather(int father[], int x) {
  if (x == father[x])
    return x;
  else
    return recursion_findFather(father, x);
}

void Union(int a_fathers[], int b_fathers[], int a, int b) {
  int faA = findFather(a_fathers, a);
  int fab = findFather(b_fathers, b);
  if (faA != fab) {
    a_fathers[faA] = fab;
  }
}

int fixed_findFather(int father[], int x) {
  int a = x;
  while (x != father[x]) {
    x = father[x];
  }
  while (a != father[a]) {
    int z = a;
    a = father[a];
    father[z] = x;
  }
  return x;
}

int plus_fixed_findFather(int father[], int v) {
  if (v == father[v])
    return v;
  else {
    int F = plus_fixed_findFather(father, v);
    father[v] = F;
    return F;
  }
}

void testfunc2() {
  const int maxn = 105;
  int friend_a[maxn] = {0};
  int friend_b[maxn] = {0};
  int father[maxn] = {0};
  int n, m;
  std::cin >> n >> m;
  for (int i = 1; i <= m; i++) {
    std::cin >> friend_a[i] >> friend_b[i];
  }
  // init union
  for (int i = 1; i <= n; i++) {
    father[i] = i;
  }
  auto findFather = [&](int x) -> int {
    while (x != father[x]) {
      x = father[x];
    }
    return x;
  };
  for (int i = 1; i <= n; i++) {
    int fa = findFather(friend_a[i]);
    int fb = findFather(friend_b[i]);
    if (fa != fb) {
      father[friend_b[i]] = fa;
    }
  }
  int last_father = 0;
  int pairs = 0;
  for (int i = 1; i <= n; i++) {
    if (last_father == 0) {
      last_father = father[i];
    } else if (last_father != father[i]) {
      pairs++;
    }
  }
  std::cout << pairs;
}

}  // namespace Union

namespace heap {
const int maxn = 100;
int heap[maxn], n = 10;

void downAdjust(int low, int high) {
  // i store the root index in the tree,j store root left-child index
  int i = low, j = i * 2;
  while (j <= high) {
    if (i + 1 <= high &&
        heap[j + 1] > heap[j]) {  // if right-child biger than left-child
      j = j + 1;                  // let j store right-child index
    }
    if (heap[j] > heap[i]) {
      // the child that weight is bigest changed with i
      std::swap(heap[j], heap[j]);
      i = j;
      j = i * 2;
    } else {
      break;
    }
  }
}

void createHeap() {
  // create heap from bottom to top
  for (int i = n / 2; i >= 1; i--) {
    downAdjust(i, n);
  }
}

void deleteTop() {
  heap[1] == heap[n--];
  downAdjust(1, n);
}

void upAdjust(int low, int high) {
  int i = high, j = i / 2;
  // j is i 's father
  while (j >= low) {
    if (heap[j] < heap[i]) {
      std::swap(heap[j], heap[i]);
      i = j;
      j = i / 2;
    } else {
      break;
    }
  }
}

void insert(int x) {
  heap[++n] = x;
  upAdjust(1, n);
}

void heapSort() {
  createHeap();
  for (int i = n; i > 1; i--) {
    std::swap(heap[i], heap[1]);
    downAdjust(1, i - 1);
  }
}
}  // namespace heap

namespace HafumantTree {
std::priority_queue<long long, std::vector<long long>, std::greater<long long>>
    q;
void createTree() {
  int n;
  long long temp, x, y, ans = 0;
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    scanf("%lld", &temp);
    q.push(temp);
  }
  while (q.size() > 1) {
    x = q.top();
    q.pop();
    y = q.top();
    q.push(x + y);
    ans += x + y;
  }
  printf("%lld\n", ans);
}
}  // namespace HafumantTree

int main() {
  using namespace Union;
  testfunc2();
  std::cout << "\nend";
}