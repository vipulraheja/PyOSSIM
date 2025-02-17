#pragma ident "$Id: Expression.hpp 1895 2009-05-12 19:34:29Z afarris $"

/**
 * @file Expression.hpp
 * Provides ability to resolve mathematical functions at runtime.
 * Class declarations.
 */

//============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 2.1 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  Copyright 2006, The University of Texas at Austin
//
//============================================================================

#ifndef EXPRESSION__HPP
#define EXPRESSION__HPP

#include <iostream>
#include <string>
#include <list>
#include <map>

#include "Exception.hpp" 

namespace gpstk
{
   /** @addtogroup math */
   //@{
 
      /**
       * This class provides the ability to resolve general mathematical
       * expressions at run time. The goal is to allow the end user the
       * ability to specify arbitrary observation transformations.
       *
       * The expression is input as a string in the constructor of the class.
       * The expression is first broken up into tokens. Those tokens are
       * prioritized according to operator precedence and the number of
       * parentheses surrounding them. This list of prioritized combinations
       * is then converted into a 'classic" binary expression tree. All this
       * happens during construction, hidden to the user.
       *
       * The design of the expression node classes is based on 
       * material by David Eck and Scotty Orr found at
       *
       *     http://math.hws.edu/orr/s04/cpsc225/btrees/index.html
       *
       * After the expression is instantiated, it can be evaluated. If the
       * expression contains variables, those must be set using the set
       * operation for the expression to successfully evaluate. 
       *
       */  

   class Expression 
   {
   public:

      NEW_EXCEPTION_CLASS(ExpressionException, gpstk::Exception);

      // Represents a node of any type in an expression tree.
      class ExpNode {
         public:

         virtual ~ExpNode() {}

         // Compute and return the numerical value of this node
         virtual double getValue()
            throw (gpstk::Expression::ExpressionException) =0;
         
  
         // Write out this node to a stream
        virtual std::ostream& print(std::ostream& ostr) =0; 

      }; // end class ExpNode


      // Represents a node that holds a number.  
      class ConstNode : public ExpNode {
         public:
            // Constructor.  Create a node to hold val.
            ConstNode( double theNum ): number(theNum) {}

            double getValue()  throw (gpstk::Expression::ExpressionException)
               { return number; }

            std::ostream& print(std::ostream& ostr) {
               ostr << number;
               return ostr;
            }

            double number;  // The number in the node.
      }; // end class ConstNode

      // Represents a node that holds a variable  
      class VarNode : public ExpNode {
         public:
            // Constructor.  

            VarNode(std::string theName ): name(theName), hasValue(false)
                {}

            double getValue()  throw (gpstk::Expression::ExpressionException);

            std::ostream& print(std::ostream& ostr) {
               ostr << name;
               return ostr;
            }

            std::string name;  // The name of the varaible
            bool hasValue;

         void setValue(double newValue);

        private:
            double value;
          
         
      }; // end class VarNode

      // Represents a node that holds an operator.
      class BinOpNode : public ExpNode {
         public:

            // Constructor.  Create a node to hold the given data.
            BinOpNode( const std::string& theOp, ExpNode *theLeft, ExpNode *theRight ):
                    op(theOp), left(theLeft), right(theRight){}

            double getValue()
               throw (gpstk::Expression::ExpressionException);

            std::ostream& print(std::ostream& ostr);

            std::string op;        // The operator.
            ExpNode *left;   // The left operand.
            ExpNode *right;  // The right operand.

      }; // end class BinOpNode

      // Represents a node that holds a function of a signle variable
      class FuncOpNode : public ExpNode {
         public:

            // Constructor.  Create a node to hold the given data.
            FuncOpNode( const std::string& theOp, ExpNode *theRight ):
                    op(theOp), right(theRight){}

            double getValue() 
               throw (gpstk::Expression::ExpressionException);

            std::ostream& print(std::ostream& ostr);

            std::string op;        // The operator.
            ExpNode *right;  // The right operand.

      }; // end class FuncOpNode

         // This class is used internally, during construction of an Expression,
         // to generate ExpNodes. 
      class Token
      {
         public:

            Token(std::string value, int relPriority, 
                  bool isOperator);

            std::string getValue(void) {return value;}

            int getPriority(void) {return priority;}

            void setUsed(void) {used=true;}
            bool getUsed(void) {return used;}

            ExpNode * getNode(void) {return expNode;}
            void setNode(ExpNode *newNode) {expNode = newNode; }

            void setResolved(bool value) {resolved=value;}
            bool getResolved(void) {return resolved;}

            bool getOperator(void) {return isOperator;}
            void setOperator(bool value) {isOperator = value;}

            std::string getArgumentPattern(void) {return argumentPattern;}
            void setArgumentPattern(std::string value) {argumentPattern = value;}

            void print(std::ostream& ostr);

         private:
            std::string value;
            bool isOperator;
            bool resolved;

            int priority;
            ExpNode *expNode;
            bool used; // has the node of this token been used (linked to?)

            std::string argumentPattern;       
      };
        
         /**
          * Constructor.
          * @param str Expression to be evaluated. 
          */
      Expression(const std::string& str);

         /// Destructor
      ~Expression(void);
      
        /**
         * Sets a variable in the expression to the input value.
         * All instances of the variable are set to this value.
         * This is required before an expression can be evaluated, if
         * the expression contains variables.
         * @param name Name of the variable to set
         * @param name Value to set the variable to.
         */
      void set(const std::string name, double value);
      void set(const char* name, double value) 
         { set (std::string(name),value); }
      
         /**
          * Returns the numerical value of the expression. Note that
          * if the expression contains variables, those variables must
          * be set.
          */
      double evaluate(void)  throw (gpstk::Expression::ExpressionException)
         { return root->getValue(); }

         /**
          * Writes the expression out to a stream.
          */
      void print(std::ostream& ostr) {root->print(ostr);}

      private:

         void tokenize(const std::string& str);
         void buildExpressionTree(void);

         int countResolvedTokens(void);   
      
         std::map<std::string,int> operatorMap;
         std::map<std::string,std::string> argumentPatternMap;
      
         std::list<Token> tList;
         std::list<ExpNode *> eList;
         ExpNode *root;      
   }; // End class expression
   
   
} // End namespace gpstk

#endif // EXPRESSION_HPP
