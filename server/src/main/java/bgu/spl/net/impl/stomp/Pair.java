package bgu.spl.net.impl.stomp;

import java.util.Objects;

public class Pair<L,R> {
    //
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by FernFlower decompiler)

        public L left;
        public R right;


        public Pair(L left, R right) {
            this.left = left;
            this.right = right;
        }

        public L getLeft() {
            return this.left;
        }

        public R getRight() {
            return this.right;
        }

        public int hashCode() {
            return Objects.hashCode(this.left) + 31 * Objects.hashCode(this.right);
        }

        public boolean equals(Object obj) {
            if (obj == this) {
                return true;
            }
            else {
                return Objects.equals(this.left, ((Pair)obj).getLeft()) && Objects.equals(this.right, ((Pair)obj).getRight());
            }
        }

        public String toString() {
            return String.format("(%s, %s)", this.left, this.right);
        }
    }

