import { useState } from 'react'

import logo from './logo.svg';
import './App.css';

function App() {
    const [products] = useState([
        {
            name: 'AA Battery',
            cost: '2.99',
            image: 'www.google.com'
        },
        {
            name: 'Blanket',
            cost: '19.99',
            image: 'www.google.com'
        },
    ]);



    return (
        <div className="App">
            <Product />
            {products.map((products) => (
                <div>
                <h3>{product.name}</h3>
                <h4>{product.cost}</h4>
                </div> 
            ))}
        </div>
    );
}

export default App;
