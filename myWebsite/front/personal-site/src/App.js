import logo from './logo.svg';
import './App.css';
import { wait } from '@testing-library/user-event/dist/utils';

import { useState, useEffect } from 'react'

function App() {

    const [data, setData] = useState([{}]); 

    useEffect(() => {
        fetch("/members").then(
            res => res.json()
        ).then(
            data => {
                setData(data)
                console.log(data)
            }
        )
    }, [])


    return (
        <>
        <div className="App">
        <header className="App-header">Josh Personal Blog</header>
        </div>
        <div>
        <ol>
        {data.map(member => (
            <li key={member}>{member}</li>
        ))}
        </ol>
        </div>
        </>
    );
}

export default App;
